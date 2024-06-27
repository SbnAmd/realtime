//
// Created by root on 6/26/24.
//

#ifndef REALTIME_PERF_MONITOR_H
#define REALTIME_PERF_MONITOR_H

#include "string.h"
#include "time.h"
#include "unistd.h"
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <asm/unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>

void get_perf_event(int core, long wait);

#endif //REALTIME_PERF_MONITOR_H
