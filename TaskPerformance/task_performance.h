//
// Created by root on 5/31/24.
//

#ifndef REALTIME_TASK_PERFORMANCE_H
#define REALTIME_TASK_PERFORMANCE_H

#include "C/tasks/benchmark_tasks.h"
#include "C/function_types/function_types.h"
#include "C/performance_counter/performance_counter.h"


#define TEST_REPETITION         3000
#define TEST_CORE               15
#define CORE_TEMP_PATH          "/sys/class/hwmon/hwmon3/temp9_input"
#define POWER_PATH              "/sys/class/powercap/intel-rapl:0/intel-rapl:0:0/energy_uj"
#define TASK_COUNT              12


void* run_test();
#endif //REALTIME_TASK_PERFORMANCE_H
