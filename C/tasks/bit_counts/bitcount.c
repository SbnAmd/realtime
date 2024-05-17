//
// Created by sbn on 5/17/24.
//

#include "bitcount.h"

static int CDECL bit_shifter(long int x)
{
    int i, n;

    for (i = n = 0; x && (i < (sizeof(long) * CHAR_BIT)); ++i, x >>= 1)
        n += (int)(x & 1L);
    return n;
}



void bitcnts(long in){
    clock_t start, stop;
    double ct, cmin = DBL_MAX, cmax = 0;
    int i, cminix, cmaxix;
    long j, n, seed;
    int iterations;
    static int (* CDECL pBitCntFunc[FUNCS])(long) = {
        bit_count,
                bitcount,
                ntbl_bitcnt,
                ntbl_bitcount,
                /*            btbl_bitcnt, DOESNT WORK*/
                BW_btbl_bitcount,
                AR_btbl_bitcount,
                bit_shifter
    };
    static char *text[FUNCS] = {
            "Optimized 1 bit/loop counter",
            "Ratko's mystery algorithm",
            "Recursive bit count by nybbles",
            "Non-recursive bit count by nybbles",
            /*            "Recursive bit count by bytes",*/
            "Non-recursive bit count by bytes (BW)",
            "Non-recursive bit count by bytes (AR)",
            "Shift and count bits"
    };

    iterations=in;



    for (i = 0; i < FUNCS; i++) {
        start = clock();

        for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13)
            n += pBitCntFunc[i](seed);

        stop = clock();
        ct = (stop - start) / (double)CLOCKS_PER_SEC;
        if (ct < cmin) {
            cmin = ct;
            cminix = i;
        }
        if (ct > cmax) {
            cmax = ct;
            cmaxix = i;
        }


    }
}



void bitcnts_large(){
    bitcnts(1125000);
}

void bitcnts_small(){
    bitcnts(75000);
}

