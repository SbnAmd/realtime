//
// Created by sbn on 5/17/24.
//

#ifndef REALTIME_BITCOUNT_H
#define REALTIME_BITCOUNT_H

#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include <limits.h>
#include <time.h>
#include <float.h>
#include "bitops.h"

#define FUNCS  7

static int CDECL bit_shifter(long int x);

void bitcnts_large();

void bitcnts_small();


#endif //REALTIME_BITCOUNT_H
