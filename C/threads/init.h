//
// Created by sbn on 5/3/24.
//

#ifndef REALTIME_INIT_H
#define REALTIME_INIT_H



#include "C/realtime.h"
#include "C/tasks/benchmark_tasks.h"
#include "C/performance_counter/performance_counter.h"
#include "C/power/power.h"
#include "C/temperature/tempertature.h"
#include "C/serdes/serdes.h"
#include "C/server/server.h"
#include "C/shared_memory/shared_memory.h"
#include "C/utils/utils.h"
#include "C/threads/manager.h"
#include "C/threads/realtime_t.h"
#include "C/threads/tick.h"



void init();


#endif //REALTIME_INIT_H
