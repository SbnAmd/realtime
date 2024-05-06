//
// Created by sbn on 5/5/24.
//

#ifndef REALTIME_PERFORMANCE_COUNTER_H
#define REALTIME_PERFORMANCE_COUNTER_H
#define GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <scheduler.h>
#include <time.h>
#include <bits/types/struct_sched_param.h>
#include "tasks/tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <string.h>


typedef void (*FunctionPtr)();

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
};


long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                     int cpu, int group_fd, unsigned long flags);

void get_cpu_event(long long* count,__u64	conf,  int cpu);
long get_memory_event();



#endif //REALTIME_PERFORMANCE_COUNTER_H
