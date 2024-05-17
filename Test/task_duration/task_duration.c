//
// Created by sbn on 5/17/24.
//

#include "task_duration.h"



void (*tasks[TASK_COUNT-1])()={&qsort_large, &qsort_small,
                                    &bitcnts_large, &bitcnts_small, &basicmath_large,
                                    &basicmath_small, &string_search_large, &string_search_small,
                                    &fft_large, &fft_small, &crc_large, &crc_small};

char* task_names[] = { "qsort_large", "qsort_small", "bitcnts_large", "bitcnts_small",
                       "basicmath_large", "basicmath_small", "string_search_large", "string_search_small",
                       "fft_large", "fft_small", "crc_large", "crc_small",

};
void* tasks_duration_worker(){

    struct timespec start, end;
    long elapsed_ns;



    for(int t = 0 ; t < TASK_COUNT-1 ; t++){
        printf("Task %s \t", task_names[t]);
        clock_gettime(CLOCK_MONOTONIC, &start);
        tasks[t]();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf(" duration : %f\n", elapsed_ns / 1000000.0);
    }


}


void check_tasks_duration(){

    struct sched_param params;
    pthread_attr_t attr;
    pthread_t thread;

    assign_task_to_core(&params, &attr, &thread, 15, tasks_duration_worker, NULL);
    pthread_join(thread, NULL);

};