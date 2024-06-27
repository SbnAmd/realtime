//
// Created by root on 6/27/24.
//

#ifndef REALTIME_CONF_H
#define REALTIME_CONF_H
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cjson/cJSON.h"

#define SET_CORE(core_id) \
    do { \
        cpu_set_t cpuset; \
        CPU_ZERO(&cpuset); \
        CPU_SET(core_id, &cpuset); \
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); \
    } while (0)




//#define BENCH_TEST

#define NUM_CORES       4
#define CORE_OFFSET     12









#endif //REALTIME_CONF_H
