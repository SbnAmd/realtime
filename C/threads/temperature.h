//
// Created by sbn on 5/21/24.
//

#ifndef REALTIME_TEMPERATURE_H
#define REALTIME_TEMPERATURE_H

#include "C/realtime.h"

#define MAX_LINE_LENGTH 16

void* temperature_worker();
void get_core_temperatures(float* core_temperatures);


#endif //REALTIME_TEMPERATURE_H
