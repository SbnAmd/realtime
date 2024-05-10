//
// Created by sbn on 5/8/24.
//

#ifndef REALTIME_SHARED_MEMORY_H
#define REALTIME_SHARED_MEMORY_H

#include "realtime.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

void* shared_mem_worker();

#endif //REALTIME_SHARED_MEMORY_H
