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
extern int kill_flag;
extern struct timespec start;


void idle(){
    usleep(PERIOD);
}

void QsortLargeTask(int core_idx){
    static char name[] = {"QsortLargeTask"};
    task(qsort_large, core_idx, name);
}

void QsortSmallTask(int core_idx){
    static char name[] = {"QsortSmallTask"};
    task(qsort_small ,core_idx, name);
}

void BitCountLargeTask(int core_idx){
    static char name[] = {"BitCountLargeTask"};
    task(bitcnts_large ,core_idx, name);
}

void BitCountSmallTask(int core_idx){
    static char name[] = {"BitCountSmallTask"};
    task(bitcnts_small ,core_idx, name);
}

void BasicMathLargeTask(int core_idx){
    static char name[] = {"BasicMathLargeTask"};
    task(basicmath_large ,core_idx, name);
}

void BasicMathSmallTask(int core_idx){
    static char name[] = {"BasicMathSmallTask"};
    task(basicmath_small    ,core_idx, name);
}

void StringSearchLargeTask(int core_idx){
    static char name[] = {"StringSearchLargeTask"};
    task(string_search_large, core_idx, name);
}

void StringSearchSmallTask(int core_idx){
    static char name[] = {"StringSearchSmallTask"};
    task(string_search_small, core_idx, name);
}

void FFTLargeTask(int core_idx){
    static char name[] = {"FFTLargeTask"};
    task(fft_large, core_idx, name);
}

void FFTSmallTask(int core_idx){
    static char name[] = {"FFTSmallTask"};
    task(fft_small, core_idx, name);
}

void CRCLargeTask(int core_idx){
    static char name[] = {"CRCLargeTask"};
    task(crc_large, core_idx, name);
}

void CRCSmallTask(int core_idx){
    static char name[] = {"CRCSmallTask"};
    task(crc_small, core_idx, name);
}

void (*task_list[TASK_COUNT])(int)={&idle, &QsortLargeTask, &QsortSmallTask,
                           &BitCountLargeTask, &BitCountSmallTask, &BasicMathLargeTask,
                           &BasicMathSmallTask, &StringSearchLargeTask, &StringSearchSmallTask,
                           &FFTLargeTask, &FFTSmallTask, &CRCLargeTask, &CRCSmallTask
                           };


void task(FunctionPtr real_task, int core_idx, char *name){

    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, name);
    struct timespec end, lstart;
    long elapsed_ns, duration_ns;

#ifdef DEBUG
//    clock_gettime(CLOCK_MONOTONIC, &end);
//    clock_gettime(CLOCK_MONOTONIC, &lstart);
//    elapsed_ns = (end.tv_sec-start.tv_sec) * 1000000000 + (end.tv_nsec-start.tv_nsec);
//    printf("\t--->[%s started on core %d at %f]\n", name, core_idx, elapsed_ns / 1000000.0);
#endif
        run_task_and_get_perf_event(real_task, perf_event, core_idx+CORE_BASE);
#ifdef DEBUG
//    clock_gettime(CLOCK_MONOTONIC, &end);
//    elapsed_ns = (end.tv_sec-start.tv_sec) * 1000000000 + (end.tv_nsec-start.tv_nsec);
//    duration_ns = (end.tv_sec-lstart.tv_sec) * 1000000000 + (end.tv_nsec-lstart.tv_nsec);
//    printf("\t--->[%s finished on core %d at %f and took %f miliseconds]\n", name, core_idx, elapsed_ns / 1000000.0, duration_ns/1000000.0);
#endif
}


void* worker(void* arg) {

    int task_idx;
    int core_idx = (int)(*((int*)arg));

    for(int i = 1; i < 13; i++)
        task_list[i](core_idx);


    // Wait until get first schedule
    LOCK(&core_mutexes[core_idx]);
    core_status[core_idx] = IDLE;
    while (new_task_stat[core_idx] == NO_TASK)
        pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
    UNLOCK(&core_mutexes[core_idx]);


    while (kill_flag == 0){

        // Change thread status
        LOCK(&core_mutexes[core_idx]);
        task_idx = new_task_IDes[core_idx];
        new_task_stat[core_idx] = NO_TASK;
        if(task_idx <= 0){
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
        while (new_task_stat[core_idx] == NO_TASK)
            pthread_cond_wait(&manage_to_core_CVes[core_idx], &core_mutexes[core_idx]);
        UNLOCK(&core_mutexes[core_idx]);
    }

    return NULL;
}