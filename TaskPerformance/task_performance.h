//
// Created by root on 5/31/24.
//

#ifndef REALTIME_TASK_PERFORMANCE_H
#define REALTIME_TASK_PERFORMANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/rand.h>
#include "C/tasks/benchmark_tasks.h"
#include "C/function_types/function_types.h"
#include "C/performance_counter/performance_counter.h"


#define TEST_REPETITION         3000

#define TEST_CORE               15
#define CORE_1                  12
#define CORE_2                  13
#define CORE_3                  14
#define CORE_4                  15

#define CORE_TEMP_PATH          "/sys/class/hwmon/hwmon3/temp9_input"
#define POWER_PATH              "/sys/class/powercap/intel-rapl:0/intel-rapl:0:0/energy_uj"
#define TASK_COUNT              12


void* run_test();
unsigned long get_core_power();
void* core_worker(int core_id);
void set_freq(int core_id, int force);
#endif //REALTIME_TASK_PERFORMANCE_H
