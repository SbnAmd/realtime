//
// Created by sbn on 5/12/24.
//

#include "tick.h"

extern pthread_cond_t tick_cond;
extern int kill_flag;
extern struct timespec start;

void* tick(void* arg){
    long n = 0;

    usleep(PRELOAD_DELAY);
    while (kill_flag == 0){

        usleep(20000);

#ifdef DEBUG
        printf("*********** Tick %ld************** \n", n);
        n++;
#endif

        pthread_cond_signal(&tick_cond);

    }
}