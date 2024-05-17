//
// Created by sbn on 5/15/24.
//

#ifndef REALTIME_PERF_COUNT_H
#define REALTIME_PERF_COUNT_H

#include "C/realtime.h"
#include "C/tasks/benchmark_tasks.h"
#include "C/function_types/function_types.h"


//typedef void (*FunctionPtr)();


void perf_count_test(FunctionPtr task, struct PerformanceEvents* perf_event, int core);

#endif //REALTIME_PERF_COUNT_H
