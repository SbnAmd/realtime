//
// Created by sbn on 5/12/24.
//

#include "realtime_t.h"


void (*task_list[7])(int)={&idle, &my_func1, &my_func2, &my_func3, &my_func4, &my_func5, &my_func6};


void task(FunctionPtr real_task, int core_idx, char *name){

    #ifdef DEBUG
        printf("%s\n", name);
    #endif

        struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
        strcpy(perf_event->name, name);
        run_task_and_get_perf_event(real_task, perf_event, core_idx);

}

void my_func1(int core_idx){
    static char name[] = {"Qsort-Large"};
    task(qsort_large, core_idx, name);
}

void my_func2(int core_idx){
    static char name[] = {"Qsort-Small"};
    task(qsort_small ,core_idx, name);
}

void my_func3(int core_idx){
    static char name[] = {"Bitcounts-Large"};
    task(bitcnts_large ,core_idx, name);
}

void my_func4(int core_idx){
static char name[] = {"Bitcounts-Small"};
    task(bitcnts_small ,core_idx, name);
}

void my_func5(int core_idx){
    static char name[] = {"Basicmath-Large"};
    task(basicmath_large ,core_idx, name);
}

void my_func6(int core_idx){
    static char name[] = {"Basicmath-Small"};
    task(basicmath_small    ,core_idx, name);
}

void idle(){
    usleep(PERIOD);
}

void* worker(void* arg) {

    int task_idx;
    int core_idx = (int)(*((int*)arg));

    // Wait until get first schedule
    LOCK(&core_mutexes[core_idx]);
    core_status[core_idx] = IDLE;
    while (new_task_stat[core_idx] == 0)
        pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
    UNLOCK(&core_mutexes[core_idx]);


    while (stop_flag == 0){

        // Change thread status
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

        // Change status to IDLE
        LOCK(&core_mutexes[core_idx]);
        core_status[core_idx] = IDLE;
        UNLOCK(&core_mutexes[core_idx]);

        // fixme
        // Notify manager, task is done
        pthread_cond_signal(&core_to_manage_CVes[core_idx]);


        // Wait until tick and next schedule
        LOCK(&core_mutexes[core_idx]);
        while (new_task_stat[core_idx] == 0)
            pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
        UNLOCK(&core_mutexes[core_idx]);

    }

    return NULL;
}