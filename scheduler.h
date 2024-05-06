//
// Created by sbn on 5/3/24.
//

#ifndef REALTIME_SCHEDULER_H
#define REALTIME_SCHEDULER_H



#include "realtime.h"
#include "tasks/tasks.h"
#include "performance_counter/performance_counter.h"
#include "power/power.h"
#include "temperature/tempertature.h"
#include "serdes/serdes.h"




void* worker(void* arg) ;
void my_func1();
void my_func2();
void my_func3();
void my_func4();
void my_func5();
void my_func6();
void idle();


#endif //REALTIME_SCHEDULER_H
