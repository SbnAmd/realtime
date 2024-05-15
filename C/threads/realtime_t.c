//
// Created by sbn on 5/12/24.
//

#include "realtime_t.h"

extern pthread_mutex_t server_mtx;
extern pthread_cond_t server_cond;
extern pthread_mutex_t core_mutexes[NUM_CORES];
extern int core_status[NUM_CORES];
extern float temperatures[TOTAL_CORES];
extern double power;
extern unsigned long energy_uj;
extern struct PerformanceEvents perf_event_array[NUM_CORES];
extern char g_buffer[2048];
extern int new_task_IDes[NUM_CORES];
extern int new_task_stat[NUM_CORES];
extern int stop_flag;
extern pthread_cond_t tick_cond;
extern pthread_mutex_t tick_mtx;
extern pthread_cond_t manage_to_core_CVes[NUM_CORES];
void (*task_list[7])(int)={&idle, &my_func1, &my_func2, &my_func3, &my_func4, &my_func5, &my_func6};
extern int kill_flag;

void task(FunctionPtr real_task, int core_idx, char *name){

        struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
        strcpy(perf_event->name, name);

//    real_task();
        run_task_and_get_perf_event(real_task, perf_event, core_idx+CORE_BASE);

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


    while (kill_flag == 0){

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

        // Wait until tick and next schedule
        LOCK(&core_mutexes[core_idx]);
        while (new_task_stat[core_idx] == 0)
            pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
        UNLOCK(&core_mutexes[core_idx]);

    }

    return NULL;
}