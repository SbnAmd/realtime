//
// Created by sbn on 5/5/24.
//

#include "performance_counter.h"



void run_task_and_get_perf_event(FunctionPtr task, struct PerformanceEvents* perf_event, int core){

    struct timespec start, end;
    struct timespec task_start, task_end;
    long task_duration_ns, total_ns, elapsed_ns;

    // Run task
    clock_gettime(CLOCK_MONOTONIC, &task_start);

    task();

    clock_gettime(CLOCK_MONOTONIC, &task_end);

    // Time calc
    elapsed_ns = (task_end.tv_sec - task_start.tv_sec) * 1000000000 + (task_end.tv_nsec - task_start.tv_nsec);
    perf_event->duration = elapsed_ns / 1000000.0; // Elapsed time in milliseconds

}

