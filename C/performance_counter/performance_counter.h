//
// Created by sbn on 5/5/24.
//

#ifndef REALTIME_PERFORMANCE_COUNTER_H
#define REALTIME_PERFORMANCE_COUNTER_H


#include "C/realtime.h"
#include "C/tasks/tasks.h"
#include "C/function_types/function_types.h"


//typedef void (*FunctionPtr)();


long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                     int cpu, int group_fd, unsigned long flags);

void run_task_and_get_perf_event(FunctionPtr task, struct PerformanceEvents* perf_event, int core);



#endif //REALTIME_PERFORMANCE_COUNTER_H
