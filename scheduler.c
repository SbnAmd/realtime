//
// Created by sbn on 5/3/24.
//


#include <scheduler.h>



static pthread_cond_t tick_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t tick_mtx = PTHREAD_MUTEX_INITIALIZER;
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
static int stop_flag = 0;
static char g_buffer[2048] = {0};




//static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

void (*task_list[7])(int)={&idle, &my_func1, &my_func2, &my_func3, &my_func4, &my_func5, &my_func6};



void my_func1(int core_idx){
    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, "Qsort-Large");
    run_task_and_get_perf_event(qsort_large, perf_event, core_idx);
}

void my_func2(int core_idx){
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

    if(pthread_mutex_lock(&core_mutexes[core_idx]) != 0){
        printf("Core %d locking failed\n", core_idx);
    }

    core_status[core_idx] = IDLE;
    while (new_task_stat[core_idx] == 0)
        pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);

    if(pthread_mutex_unlock(&core_mutexes[core_idx]) != 0){
        printf("Core %d unlocking failed\n", core_idx);
    }

    while (stop_flag == 0){

        if(pthread_mutex_lock(&core_mutexes[core_idx]) != 0){
            printf("Core %d locking failed\n", core_idx);
        }

        // Task selection
        task_idx = new_task_IDes[core_idx];
        new_task_stat[core_idx] = 0;
        if(task_idx == 0){
            core_status[core_idx] = IDLE;
        } else{
            core_status[core_idx] = RUNNING;
        }

        if(pthread_mutex_unlock(&core_mutexes[core_idx]) != 0){
            printf("Core %d unlocking failed\n", core_idx);
        }

        // Run task
        task_list[task_idx](core_idx);


        if(pthread_mutex_lock(&core_mutexes[core_idx]) != 0){
            printf("Core %d locking failed\n", core_idx);
        }
        // Set to IDLE
        core_status[core_idx] = IDLE;

        if(pthread_mutex_unlock(&core_mutexes[core_idx]) != 0){
            printf("Core %d unlocking failed\n", core_idx);
        }

        // fixme
        // Notify task is done
        pthread_cond_signal(&core_to_manage_CVes[core_idx]);


        // Wait until tick and decide what to do
        if(pthread_mutex_lock(&core_mutexes[core_idx]) != 0){
            printf("Core %d locking failed\n", core_idx);
        }

        while (new_task_stat[core_idx] == 0)
            pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);

        if(pthread_mutex_unlock(&core_mutexes[core_idx]) != 0){
            printf("Core %d unlocking failed\n", core_idx);
        }
    }

    return NULL;
}

void* manager(void* arg){

    int status[NUM_CORES];
    int new_tasks[NUM_CORES];

    while (stop_flag == 0){

        for(int i = 0; i < NUM_CORES; i++){
            if(pthread_mutex_lock(&core_mutexes[i]) != 0){
                printf("Core %d locking failed\n", i);
                i--;
                continue;
            }
            // Copying core statuses
            status[i] = core_status[i];
            if(pthread_mutex_unlock(&core_mutexes[i]) != 0){
                printf("Core %d unlocking failed\n", i);
            }
        }

        get_core_temperatures(temperatures);
        get_power_and_energy(&power, &energy_uj);

        //serialize
        serialize(perf_event_array, g_buffer, NUM_CORES, &power, &energy_uj, temperatures);

        // todo: send status
        // todo: wait until new schedule


        for(int i = 0; i < NUM_CORES; i++){
            if(status[i] == IDLE){
                if(pthread_mutex_lock(&core_mutexes[i]) != 0){
                    printf("Core %d locking failed\n", i);
                    i--;
                    continue;
                }
                new_task_IDes[i] = new_tasks[i];
                new_task_stat[i] = 1;
                printf("Task_ID[%d] = %d\n", i, new_tasks[i]);
                pthread_cond_signal(&manage_to_core_CVes[i]);
                if(pthread_mutex_unlock(&core_mutexes[i]) != 0){
                    printf("Core %d unlocking failed\n", i);
                }
            }
        }


        if(pthread_mutex_lock(&tick_mtx) != 0){
            printf("Tick locking failed\n");
        }

        pthread_cond_wait(&tick_cond, &tick_mtx);

        if(pthread_mutex_unlock(&tick_mtx) != 0){
            printf("Tick unlocking failed\n");
        }

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
    pthread_attr_t attrs[NUM_CORES], tick_attr, manager_attr;
    pthread_t threads[NUM_CORES], tick_thread, manager_thread;


    // Init

    for(int i =0 ; i < NUM_CORES; i++)
        core_IDes[i] = i;

    for(int i = 0; i < NUM_CORES; i++){
        // Set real-time scheduling parameters
        params.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_attr_init(&attrs[i]);
        pthread_attr_setinheritsched(&attrs[i], PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attrs[i], SCHED_FIFO);
        pthread_attr_setschedparam(&attrs[i], &params);

        if (pthread_create(&threads[i], &attrs[i], worker, &core_IDes[i]) != 0){
            printf("Thread creation failed\n");
        }
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i + CORE_BASE, &cpuset);
        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);

    }
    // ************************************************************ //
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_attr_init(&manager_attr);
    pthread_attr_setinheritsched(&manager_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&manager_attr, SCHED_FIFO);
    pthread_attr_setschedparam(&manager_attr, &params);

    if (pthread_create(&manager_thread, &manager_attr, manager, NULL) != 0){
        printf("Manager thread failed\n");
    }

    cpu_set_t manager_cpuset;
    CPU_ZERO(&manager_cpuset);
    CPU_SET(15, &manager_cpuset);
    pthread_setaffinity_np(manager_thread, sizeof(cpu_set_t), &manager_cpuset);

    // ************************************************************ //
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_attr_init(&tick_attr);
    pthread_attr_setinheritsched(&tick_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&tick_attr, SCHED_FIFO);
    pthread_attr_setschedparam(&tick_attr, &params);

    if (pthread_create(&tick_thread, &tick_attr, tick, NULL) != 0){
        printf("Tick thread failed\n");
    }

    cpu_set_t tick_cpuset;
    CPU_ZERO(&tick_cpuset);
    CPU_SET(14, &tick_cpuset);
    pthread_setaffinity_np(tick_thread, sizeof(cpu_set_t), &tick_cpuset);

    // ************************************************************ //

    // ************************************************************ //

    usleep(20000000);
    stop_flag = 1;


    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }

    pthread_join(manager_thread, NULL);
    pthread_attr_destroy(&manager_attr);

    pthread_join(tick_thread, NULL);
    pthread_attr_destroy(&tick_attr);

}

int main(){

    printf("%lu", sizeof(perf_event_array ));

    return 0;
}