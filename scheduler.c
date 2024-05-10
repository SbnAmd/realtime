//
// Created by sbn on 5/3/24.
//


#include <scheduler.h>



static pthread_cond_t tick_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t tick_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t server_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t manage_to_core_CVes[NUM_CORES] = {PTHREAD_COND_INITIALIZER};
static pthread_cond_t core_to_manage_CVes[NUM_CORES] = {PTHREAD_COND_INITIALIZER};
static pthread_mutex_t core_mutexes[NUM_CORES] = {PTHREAD_MUTEX_INITIALIZER};
static int new_task_IDes[NUM_CORES] = {0};
static int new_task_stat[NUM_CORES] = {0};
static int core_status[NUM_CORES]={IDLE};
static int core_IDes[NUM_CORES];
static struct PerformanceEvents perf_event_array[NUM_CORES] = {0};
static float temperatures[TOTAL_CORES] = {0};
static double power;
static unsigned long energy_uj;
int stop_flag = 0;
char g_buffer[2048] = {0};




//static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

void (*task_list[7])(int)={&idle, &my_func1, &my_func2, &my_func3, &my_func4, &my_func5, &my_func6};



void my_func1(int core_idx){
#ifdef DEBUG
    printf("Qsort-Large\n");
#endif
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Qsort-Large");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void my_func2(int core_idx){
#ifdef DEBUG
    printf("Qsort-Small\n");
#endif
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Qsort-Small");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void my_func3(int core_idx){
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Bitcounts-Large");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void my_func4(int core_idx){
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Bitcounts-Small");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void my_func5(int core_idx){
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Basicmath-Large");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void my_func6(int core_idx){
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Basicmath-Small");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void idle(){
    usleep(20000);
}

void* worker(void* arg) {
    int task_idx;

    int core_idx = (int)(*((int*)arg));

    LOCK(&core_mutexes[core_idx]);
    core_status[core_idx] = IDLE;
    while (new_task_stat[core_idx] == 0)
        pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
    UNLOCK(&core_mutexes[core_idx]);


    while (stop_flag == 0){

        // Task selection
        LOCK(&core_mutexes[core_idx]);
        task_idx = new_task_IDes[core_idx];
        new_task_stat[core_idx] = 0;
        if(task_idx == 0){
            core_status[core_idx] = IDLE;
        } else{
            core_status[core_idx] = RUNNING;
        }
        UNLOCK(&core_mutexes[core_idx]);

        // Run task
        task_list[task_idx](core_idx);

        // Set to IDLE
        LOCK(&core_mutexes[core_idx]);
        core_status[core_idx] = IDLE;
        UNLOCK(&core_mutexes[core_idx]);

        // fixme
        // Notify task is done
        pthread_cond_signal(&core_to_manage_CVes[core_idx]);


        // Wait until tick and decide what to do
        LOCK(&core_mutexes[core_idx]);
        while (new_task_stat[core_idx] == 0)
            pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
        UNLOCK(&core_mutexes[core_idx]);

    }

    return NULL;
}

void* manager(void* arg){

    int status[NUM_CORES];
    int new_tasks[NUM_CORES];

    WAIT_ON_LOCK(&server_mtx, &server_cond);

    while (stop_flag == 0){

        for(int i = 0; i < NUM_CORES; i++){

            // Copying core statuses
            LOCK(&core_mutexes[i]);
            status[i] = core_status[i];
            UNLOCK(&core_mutexes[i]);
        }
#ifdef DEBUG
        struct timespec start, end;
        long elapsed_ns;
#endif

#ifdef DEBUG
        clock_gettime(CLOCK_MONOTONIC, &start);
#endif
        get_core_temperatures(temperatures);
#ifdef DEBUG
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("temp get time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds
#endif
        get_power_and_energy(&power, &energy_uj);

        //serialize
        serialize(perf_event_array, g_buffer, NUM_CORES, &power, &energy_uj, temperatures);

        // todo: send status

        pthread_cond_signal(&server_cond);

        // Wait until new schedule
        WAIT_ON_LOCK(&server_mtx, &server_cond);

        // Deserialize
        // fixme: if there is more than 9 tasks, 2 digit ID
        for(int j=0; j < NUM_CORES; j++){
            char c = g_buffer[j];
            new_tasks[j] = atoi(&c);
        }



        for(int i = 0; i < NUM_CORES; i++) {
            if (status[i] == IDLE) {
                LOCK(&core_mutexes[i]);
                new_task_IDes[i] = new_tasks[i];
                new_task_stat[i] = 1;
                pthread_cond_signal(&manage_to_core_CVes[i]);
                UNLOCK(&core_mutexes[i]);
            }
        }
        WAIT_ON_LOCK(&tick_mtx, &tick_cond);

    }

}


void* tick(void* arg){

    while (stop_flag == 0){

        usleep(20000);

        pthread_cond_signal(&tick_cond);

    }
}


void init_cores(){

    struct sched_param params;
    pthread_attr_t attrs[NUM_CORES], tick_attr, manager_attr, shared_mem_thread_attr;
    pthread_t threads[NUM_CORES], tick_thread, manager_thread, shared_mem_thread;


    // Init
    for(int i =0 ; i < NUM_CORES; i++)
        core_IDes[i] = i;

    // Create worker threads for tasks on 4 cores
    for(int i = 0; i < NUM_CORES; i++){
        assign_task_to_core(&params, &attrs[i], &threads[i], i + CORE_BASE, worker, &core_IDes[i]);
    }

    // Create manager task
    assign_task_to_core(&params, &manager_attr, &manager_thread, 15, manager, NULL);

    // Create tick thread
    assign_task_to_core(&params, &tick_attr, &tick_thread, 14, tick, NULL);

    // Create shared mem thread
    assign_task_to_core(&params, &shared_mem_thread_attr, &shared_mem_thread, 13, shared_mem_worker, NULL);


    // Run for 20 secs
    usleep(20000000);
    stop_flag = 1;

    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }

    // Release all
    pthread_join(manager_thread, NULL);
    pthread_attr_destroy(&manager_attr);

    pthread_join(tick_thread, NULL);
    pthread_attr_destroy(&tick_attr);
    pthread_join(shared_mem_thread, NULL);
    pthread_attr_destroy(&shared_mem_thread_attr);

}

int main(){

    init_cores();

    return 0;
}