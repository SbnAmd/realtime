//
// Created by sbn on 5/4/24.
//

#ifndef REALTIME_TASKS_H
#define REALTIME_TASKS_H

#include <math.h>
#include "snipmath.h"
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include <limits.h>
#include <time.h>
#include <float.h>
#include "bitops.h"

#define FUNCS  7
#define UNLIMIT
#define MAXARRAY 60000

struct myStringStruct {
    char qstring[128];
};

struct my3DVertexStruct {
    int x, y, z;
    double distance;
};

void qsort_large();
void qsort_small();

void bitcnts_large();
void bitcnts_small();

void basicmath_large();
void basicmath_small();




#endif //REALTIME_TASKS_H
