//
// Created by sbn on 5/6/24.
//

#ifndef REALTIME_POWER_H
#define REALTIME_POWER_H

#define GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <scheduler.h>
#include <time.h>
#include <bits/types/struct_sched_param.h>
#include "tasks/tasks.h"
#define MAX_BUF_SIZE 256

#define RAPL_PATH "/sys/class/powercap/intel-rapl:0/intel-rapl:0:0/energy_uj"


unsigned long get_cores_power();
void get_energy_consumption(unsigned long* energy_uj);


#endif //REALTIME_POWER_H
