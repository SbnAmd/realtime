//
// Created by sbn on 5/3/24.
//


#include "scheduler.h"
#include "string.h"

extern int core_IDes[NUM_CORES];
extern int stop_flag;

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


    // Run for 1000 cycles
    usleep(1000*PERIOD);
    stop_flag = 1;

    printf("waiting to join\n");

    pthread_join(manager_thread, NULL);
    pthread_attr_destroy(&manager_attr);
    printf("manager joined\n");

    pthread_join(tick_thread, NULL);
    printf("tick joined\n");
    pthread_attr_destroy(&tick_attr);

    if (pthread_cancel(shared_mem_thread) != 0) {
        fprintf(stderr, "Failed to cancel thread\n");
    }
    pthread_join(shared_mem_thread, NULL);
    printf("shared mem joined\n");
    pthread_attr_destroy(&shared_mem_thread_attr);

    for(int i = 0; i < NUM_CORES; i++){
        if (pthread_cancel(threads[i]) != 0) {
            fprintf(stderr, "Failed to cancel thread\n");
        }
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }
    printf("workers joined\n");

    // Release all




}

int main(){
    init_cores();

//    qsort_small();
//    qsort_large();
//    bitcnts_large();
//    bitcnts_small();
//    basicmath_small();
//    basicmath_large();

    return 0;
}