//
// Created by sbn on 5/3/24.
//


#include <scheduler.h>


int tasks[NUM_TASKS];
int tasks_completed = 0;
int results[NUM_TASKS];
int thread_status[NUM_TASKS] = {0,0,0,0};

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t task_var_mtx = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

void (*task_list[6])()={&my_func1, &my_func2, &my_func3, &my_func4, &my_func5, &idle};

int task_status=0;
int new_task=0;
int shared_task_index;

void my_func1(){
    long cnt;
    struct timespec start, end;
    long elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Task running part1\n");

    for(long i = 0; i < 10000000000; i++)
        cnt += 1;

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double elapsed_ms = elapsed_ns / 1000000.0; // Elapsed time in milliseconds
    printf("Elapsed time by task1: %.2f milliseconds\n", elapsed_ms);
}

void my_func2(){
    long cnt;
    struct timespec start, end;
    long elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Task running part2\n");

    for(long i = 0; i < 11000000000; i++)
        cnt += 1;

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double elapsed_ms = elapsed_ns / 1000000.0; // Elapsed time in milliseconds
    printf("Elapsed time by task2: %.2f milliseconds\n", elapsed_ms);
}
void my_func3(){
    long cnt;
    struct timespec start, end;
    long elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Task running part3\n");

    for(long i = 0; i < 6000000000; i++)
        cnt += 1;

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double elapsed_ms = elapsed_ns / 1000000.0; // Elapsed time in milliseconds
    printf("Elapsed time by task3: %.2f milliseconds\n", elapsed_ms);
}
void my_func4(){
    long cnt;
    struct timespec start, end;
    long elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Task running part4\n");

    for(long i = 0; i < 5000000000; i++)
        cnt += 1;

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double elapsed_ms = elapsed_ns / 1000000.0; // Elapsed time in milliseconds
    printf("Elapsed time by task4: %.2f milliseconds\n", elapsed_ms);
}
void my_func5(){
    long cnt;
    struct timespec start, end;
    long elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Task running part5\n");

    for(long i = 0; i < 2000000000; i++)
        cnt += 1;

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double elapsed_ms = elapsed_ns / 1000000.0; // Elapsed time in milliseconds
    printf("Elapsed time by task5: %.2f milliseconds\n", elapsed_ms);
}

void idle(){
    usleep(9999);
}

void* worker(void* arg) {
    int ret, task_idx;
//    my_func1();
//    my_func2();



    while (1){ // todo: change condition

        // Check and select task
        ret = pthread_mutex_lock(&task_var_mtx);
        if (ret != 0)
            printf("Thread failed to select task\n");

        // Task selection
        switch (shared_task_index) {

            case 0:
                task_idx = 0;
                break;
            case 1:
                task_idx = 1;
                break;
            case 2:
                task_idx = 2;
                break;
            case 3:
                task_idx = 3;
                break;
            case 4:
                task_idx = 4;
                break;
            default:
                task_idx = 5;
                break;

        }

        new_task = 0;
        task_status = 1;
//        printf("Task idx : %d\n", task_idx);
        ret = pthread_mutex_unlock(&task_var_mtx);
        if (ret != 0)
            printf("Task selection mtx unlock failed\n");

        // Run task
        task_list[task_idx]();

        // Notify task is done
        ret = pthread_mutex_lock(&task_var_mtx);
        if (ret != 0)
            printf("Thread failed to select task\n");
        task_status = 0;
//        printf("Task status : %d\n", task_status);
        ret = pthread_mutex_unlock(&task_var_mtx);
        if (ret != 0)
            printf("Task selection mtx unlock failed\n");


        // Wait until tick and decide what to do
        ret = pthread_mutex_lock(&task_var_mtx);
        if (ret != 0)
            printf("pthread_mutex_lock failed\n");

        while (new_task==0)
            pthread_cond_wait(&threadDied, &task_var_mtx);

        ret = pthread_mutex_unlock(&task_var_mtx);
        if (ret != 0)
            printf("pthread_mutex_unlock failed\n");
    }

    return NULL;
}


void init_cores(){
    struct sched_param params;
    pthread_attr_t attrs[NUM_CORES];
    pthread_t threads[NUM_CORES];
    int ret;
    unsigned int cnt=0;


    // Init
    shared_task_index = 0;
    new_task = 1;

//    for(int i = 0; i < NUM_CORES; i++){
    for(int i = 0; i < 1; i++){
        // Set real-time scheduling parameters
        params.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_attr_init(&attrs[i]);
        pthread_attr_setinheritsched(&attrs[i], PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attrs[i], SCHED_FIFO);
        pthread_attr_setschedparam(&attrs[i], &params);

        if (pthread_create(&threads[i], &attrs[i], worker, NULL) != 0){
            printf("Thread creation failed\n");
        }

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i + CORE_BASE, &cpuset);
        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);

    }

    while (1){
        cnt++;
//        printf("Tick : %d\n", cnt);
        usleep(10000);

        ret = pthread_mutex_lock(&task_var_mtx);
        if (ret != 0)
            printf("Thread failed to select task\n");

        if(task_status == 0){
            shared_task_index = (int)cnt%6;
            new_task = 1;
//            printf("Shared task index : %d\n", shared_task_index);
        }

        pthread_cond_signal(&threadDied);

        ret = pthread_mutex_unlock(&task_var_mtx);
        if (ret != 0)
            printf("Task selection mtx unlock failed\n");

    }


    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }

}

int main(){

    init_cores();

    return 0;
}