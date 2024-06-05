//
// Created by sbn on 5/6/24.
//

#ifndef REALTIME_POWER_H
#define REALTIME_POWER_H


#include "C/realtime.h"
#include "C/tasks/benchmark_tasks.h"
#define MAX_BUF_SIZE 256

#define RAPL_PATH "/sys/Class/powercap/intel-rapl:0/intel-rapl:0:0/energy_uj"


unsigned long get_cores_power();
void get_energy_consumption(unsigned long* energy_uj);
void get_power_and_energy(double* power,unsigned long* energy_uj);


#endif //REALTIME_POWER_H
