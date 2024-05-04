//
// Created by sbn on 5/3/24.
//

#ifndef REALTIME_SCHEDULER_H
#define REALTIME_SCHEDULER_H

#define GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <scheduler.h>
#include <time.h>
#include <bits/types/struct_sched_param.h>

#define NUM_CORES 8
#define CORE_BASE 8
#define NUM_TASKS 20


typedef struct {
    int task_index;
    int core_index;
} TaskArgs;

void* worker(void* arg) ;
void my_func1();
void my_func2();
void my_func3();
void my_func4();
void my_func5();
void idle();


#endif //REALTIME_SCHEDULER_H
