//
// Created by sbn on 5/3/24.
//


#include "init.h"
#include "string.h"

extern int core_IDes[NUM_CORES];
extern int stop_flag;

struct sched_param params;
pthread_attr_t attrs[NUM_CORES], tick_attr, manager_attr, shared_mem_thread_attr;
pthread_t threads[NUM_CORES], tick_thread, manager_thread, shared_mem_thread;


void init_threads(){

    for(int i =0 ; i < NUM_CORES; i++)
        core_IDes[i] = i;

    // Create worker threads for tasks on 4 realtime cores 8, 9, 10, 11
    for(int i = 0; i < NUM_CORES; i++){
        assign_task_to_core(&params, &attrs[i], &threads[i], i + CORE_BASE, worker, &core_IDes[i]);
    }

    // Create manager thread on core 15
    assign_task_to_core(&params, &manager_attr, &manager_thread, 15, manager, NULL);

    // Create tick thread on core 14
    assign_task_to_core(&params, &tick_attr, &tick_thread, 14, tick, NULL);

    // Create shared mem thread on core 13
    assign_task_to_core(&params, &shared_mem_thread_attr, &shared_mem_thread, 13, shared_mem_worker, NULL);

}

void release_threads(){

    // Release Manager thread
    pthread_join(manager_thread, NULL);
    pthread_attr_destroy(&manager_attr);

    // Release Tick thread
    pthread_join(tick_thread, NULL);
    pthread_attr_destroy(&tick_attr);

    // Release Shared memory thread
    if (pthread_cancel(shared_mem_thread) != 0) {
        fprintf(stderr, "Failed to cancel thread\n");
    }
    pthread_join(shared_mem_thread, NULL);
    pthread_attr_destroy(&shared_mem_thread_attr);

    // Release realtime cores thread
    for(int i = 0; i < NUM_CORES; i++){
        if (pthread_cancel(threads[i]) != 0) {
            fprintf(stderr, "Failed to cancel thread\n");
        }
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&attrs[i]);
    }
    printf("All threads joined\n");

}

void init(){

    // create and run pthreads [realtime cores threads, manager thread, tick thread, shared memory thread]
    init_threads();

    // Run for 1000 cycles
    usleep(1000*PERIOD);
    stop_flag = 1;

    // Release all
    release_threads();
}

