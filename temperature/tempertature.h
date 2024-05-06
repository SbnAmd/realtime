//
// Created by sbn on 5/6/24.
//

#ifndef REALTIME_TEMPERTATURE_H
#define REALTIME_TEMPERTATURE_H


#include <stdio.h>
#include "sensors/sensors.h"
#include <string.h>

#define MAX_LINE_LENGTH 256

#define CORES_NUM 8 // Assuming 8 cores, adjust accordingly

void get_core_temperatures(float* core_temperatures);


#endif //REALTIME_TEMPERTATURE_H
