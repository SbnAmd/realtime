//
// Created by sbn on 5/6/24.
//

#ifndef REALTIME_SERDES_H
#define REALTIME_SERDES_H

#include "C/realtime.h"


void serialize(struct PerformanceEvents *events, char *buffer,
               int num_events, double* power, unsigned long* energy, float * temperatures, int* status);
void deserialize(int*, char*);

#endif //REALTIME_SERDES_H
