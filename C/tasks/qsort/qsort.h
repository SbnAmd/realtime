//
// Created by sbn on 5/17/24.
//

#ifndef REALTIME_QSORT_H
#define REALTIME_QSORT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define UNLIMIT
#define MAXARRAY 60000 /* this number, if too large, will cause a seg. fault!! */

struct my3DVertexStruct {
    int x, y, z;
    double distance;
};

struct myStringStruct {
    char qstring[128];
};

void qsort_small();
void qsort_large();

#endif //REALTIME_QSORT_H
