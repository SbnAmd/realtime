//
// Created by sbn on 5/4/24.
//

#include "tasks.h"


static int CDECL bit_shifter(long int x)
{
    int i, n;

    for (i = n = 0; x && (i < (sizeof(long) * CHAR_BIT)); ++i, x >>= 1)
        n += (int)(x & 1L);
    return n;
}




int compare(const void *elem1, const void *elem2)
{
    int result;

    result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);

    return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}


int l_compare(const void *elem1, const void *elem2)
{
    /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
    /* sort based on distances from the origin... */

    double distance1, distance2;

    distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
    distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

    return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}



void basicmath_small(){

    double  a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
    double  a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
    double  a3 = 1.0, b3 = -3.5, c3 = 22.0, d3 = -31.0;
    double  a4 = 1.0, b4 = -13.7, c4 = 1.0, d4 = -35.0;
    double  x[3];
    double X;
    int     solutions;
    int i;
    unsigned long l = 0x3fed0169L;
    struct int_sqrt q;
    long n = 0;

    /* should get 3 solutions: 2, 6 & 2.5   */
    SolveCubic(a1, b1, c1, d1, &solutions, x);

    /* should get 1 solution: 2.5           */
    SolveCubic(a2, b2, c2, d2, &solutions, x);

    SolveCubic(a3, b3, c3, d3, &solutions, x);

    SolveCubic(a4, b4, c4, d4, &solutions, x);

    /* Now solve some random equations */
    for(a1=1;a1<10;a1++) {
        for (b1 = 10; b1 > 0; b1--) {
            for (c1 = 5; c1 < 15; c1 += 0.5) {
                for (d1 = -1; d1 > -11; d1--) {
                    SolveCubic(a1, b1, c1, d1, &solutions, x);
                }
            }
        }
    }

    /* perform some integer square roots */
    for (i = 0; i < 1001; ++i)
    {
        usqrt(i, &q);

    }
    usqrt(l, &q);


}


void basicmath_large(){

    double  a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
    double  x[3];
    double X;
    int     solutions;
    int i;
    unsigned long l = 0x3fed0169L;
    struct int_sqrt q;
    long n = 0;

    /* solve soem cubic functions */

    /* should get 3 solutions: 2, 6 & 2.5   */
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = 1.0; b1 = -4.5; c1 = 17.0; d1 = -30.0;
    /* should get 1 solution: 2.5           */
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = 1.0; b1 = -3.5; c1 = 22.0; d1 = -31.0;
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = 1.0; b1 = -13.7; c1 = 1.0; d1 = -35.0;
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = 3.0; b1 = 12.34; c1 = 5.0; d1 = 12.0;
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = -8.0; b1 = -67.89; c1 = 6.0; d1 = -23.6;
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = 45.0; b1 = 8.67; c1 = 7.5; d1 = 34.0;
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    a1 = -12.0; b1 = -1.7; c1 = 5.3; d1 = 16.0;
    SolveCubic(a1, b1, c1, d1, &solutions, x);


    /* Now solve some random equations */
    for(a1=1;a1<10;a1+=1) {
        for(b1=10;b1>0;b1-=.25) {
            for(c1=5;c1<15;c1+=0.61) {
                for(d1=-1;d1>-5;d1-=.451) {
                    SolveCubic(a1, b1, c1, d1, &solutions, x);

                }
            }
        }
    }



    /* perform some integer square roots */
    for (i = 0; i < 100000; i+=2)
    {
        usqrt(i, &q);
        // remainder differs on some machines
        // printf("sqrt(%3d) = %2d, remainder = %2d\n",

    }

    for (l = 0x3fed0169L; l < 0x3fed4169L; l++)
    {
        usqrt(l, &q);
        //printf("\nsqrt(%lX) = %X, remainder = %X\n", l, q.sqrt, q.frac);

    }


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


void qsort_small(){

    struct myStringStruct array[MAXARRAY];
    FILE *fp;
    int i,count=0;


    fp = fopen("/home/sbn/Desktop/realtime/C/tasks/input_small.dat","r");

    while((fscanf(fp, "%s", &array[count].qstring) == 1) && (count < MAXARRAY)) {
        count++;
    }

    qsort(array,count,sizeof(struct myStringStruct),compare);


}


void qsort_large(){

    struct my3DVertexStruct array[MAXARRAY];
    FILE *fp;
    int i,count=0;
    int x, y, z;

    if (0) {

    }
    else {
        fp = fopen("/home/sbn/Desktop/realtime/C/tasks/input_large.dat","r");

        while((fscanf(fp, "%d", &x) == 1) && (fscanf(fp, "%d", &y) == 1) && (fscanf(fp, "%d", &z) == 1) &&  (count < MAXARRAY)) {
            array[count].x = x;
            array[count].y = y;
            array[count].z = z;
            array[count].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
            count++;
        }
    }

    qsort(array,count,sizeof(struct my3DVertexStruct),l_compare);


}
