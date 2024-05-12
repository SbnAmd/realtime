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
#include <arpa/inet.h>
#include <cjson/cJSON.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>




#define NUM_CORES 4
#define TOTAL_CORES 8
#define CORE_BASE  12
#define RUNNING 1
#define PERIOD 20000
#define IDLE 0
#define MAX_LENGTH 256

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


pthread_cond_t tick_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t tick_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t server_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t manage_to_core_CVes[NUM_CORES] = {PTHREAD_COND_INITIALIZER};
static pthread_cond_t core_to_manage_CVes[NUM_CORES] = {PTHREAD_COND_INITIALIZER};
static pthread_mutex_t core_mutexes[NUM_CORES] = {PTHREAD_MUTEX_INITIALIZER};
static int new_task_IDes[NUM_CORES] = {0};
static int new_task_stat[NUM_CORES] = {0};
static int core_status[NUM_CORES]={IDLE};
static int core_IDes[NUM_CORES];
static struct PerformanceEvents perf_event_array[NUM_CORES] = {0};
static float temperatures[TOTAL_CORES] = {0};
static double power;
static unsigned long energy_uj;
int stop_flag = 0;
char g_buffer[2048] = {0};


#endif //REALTIME_REALTIME_H
