//
// Created by sbn on 5/12/24.
//

#ifndef REALTIME_REALTIME_T_H
#define REALTIME_REALTIME_T_H

#include "C/realtime.h"
#include "C/tasks/tasks.h"
#include "C/performance_counter/performance_counter.h"
#include "C/function_types/function_types.h"




void* worker(void* arg) ;
void my_func1();
void my_func2();
void my_func3();
void my_func4();
void my_func5();
void my_func6();
void idle();

void task(FunctionPtr real_task, int core_idx, char *name);




#endif //REALTIME_REALTIME_T_H
