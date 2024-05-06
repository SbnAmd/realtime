//
// Created by sbn on 5/6/24.
//

#ifndef REALTIME_REALTIME_H
#define REALTIME_REALTIME_H

#define GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <scheduler.h>
#include <time.h>
#include <bits/types/struct_sched_param.h>
#include <string.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>



#define NUM_CORES 4
#define TOTAL_CORES 8
#define CORE_BASE  12
#define RUNNING 1
#define IDLE 0
#define MAX_LENGTH 256



struct PerformanceEvents{
    int core_idx;
    long long cpu_cycles;
    long long cpu_instructions;
    long long cpu_cache_misses;
    long long cpu_cache_references;
    long long cpu_branch_misses;
    long long cpu_branch_instructions;
    long long cpu_page_faults;
    long long cpu_context_switches;
    long long cpu_migrations;
    double duration;
    char name[MAX_LENGTH];
};


#endif //REALTIME_REALTIME_H
