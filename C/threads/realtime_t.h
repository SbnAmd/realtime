//
// Created by sbn on 5/12/24.
//

#ifndef REALTIME_REALTIME_T_H
#define REALTIME_REALTIME_T_H

#include "C/realtime.h"
#include "C/tasks/benchmark_tasks.h"
#include "C/performance_counter/performance_counter.h"
#include "C/function_types/function_types.h"




void* worker(void* arg) ;
void QsortLargeTask(int);
void QsortSmallTask(int);
void BitCountLargeTask(int);
void BitCountSmallTask(int);
void BasicMathLargeTask(int);
void BasicMathSmallTask(int);
void StringSearchLargeTask(int);
void StringSearchSmallTask(int);
void FFTLargeTask(int);
void FFTSmallTask(int);
void CRCLargeTask(int);
void CRCSmallTask(int);
void idle();

void task(FunctionPtr real_task, int core_idx, char *name);


#endif //REALTIME_REALTIME_T_H
