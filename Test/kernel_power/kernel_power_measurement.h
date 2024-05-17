//
// Created by sbn on 5/16/24.
//

#ifndef REALTIME_KERNEL_POWER_MEASUREMENT_H
#define REALTIME_KERNEL_POWER_MEASUREMENT_H

#include "C/realtime.h"

#define     BEGIN_CORE              4
#define     END_CORE                15
#define     CORE_OFFSET             BEGIN_CORE
#define     SLEEPING_CORES          (END_CORE - BEGIN_CORE + 1)
#define     MEASURE_DURATION        (1000*1)
#define     REPETITION              10000
#define     SLEEP_TIME              (30*1000*MEASURE_DURATION)          // 30 seconds

void measure_kernel_power();
#endif //REALTIME_KERNEL_POWER_MEASUREMENT_H
