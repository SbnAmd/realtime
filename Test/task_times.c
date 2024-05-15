//
// Created by sbn on 5/15/24.
//

#include "task_times.h"



void tasks_time(){

    struct timespec start, end;
    long elapsed_ns;


    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort_large();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("qsort_large time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds


    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort_small();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("qsort_small time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds



    clock_gettime(CLOCK_MONOTONIC, &start);
    bitcnts_small();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("bitcnts_small time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds


    clock_gettime(CLOCK_MONOTONIC, &start);
    bitcnts_large();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("bitcnts_large time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds


    clock_gettime(CLOCK_MONOTONIC, &start);
    basicmath_large();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("basicmath_large time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds


    clock_gettime(CLOCK_MONOTONIC, &start);
    basicmath_small();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("basicmath_small time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds

}
