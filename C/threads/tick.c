//
// Created by sbn on 5/12/24.
//

#include "tick.h"

extern pthread_cond_t tick_cond;
extern int kill_flag;
extern struct timespec start;
extern pthread_cond_t temperature_cond;

void* tick(void* arg){
    long n = 0;

    usleep(PRELOAD_DELAY);
    while (kill_flag == 0){

        usleep(10000);


        pthread_cond_signal(&temperature_cond);
        usleep(10000);
#ifdef DEBUG
        printf("*********** Tick %ld************** \n", n);
        n++;
#endif


        pthread_cond_signal(&tick_cond);

    }
}