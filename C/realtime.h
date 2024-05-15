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
#include "C/threads/init.h"
#include <time.h>
#include <bits/types/struct_sched_param.h>
#include <string.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <arpa/inet.h>
#include <cjson/cJSON.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>




#define NUM_CORES 4
#define TOTAL_CORES 16
#define CORE_BASE  8
#define RUNNING 1
#define PERIOD 20000
#define IDLE 0
#define MAX_LENGTH 256
#define CYCLES_TO_RUN 10


#define DEBUG



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
