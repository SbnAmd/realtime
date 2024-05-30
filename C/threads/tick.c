//
// Created by sbn on 5/12/24.
//

#include "tick.h"

extern pthread_cond_t tick_cond;
extern pthread_mutex_t tick_mtx;
extern int kill_flag;
extern struct timespec start;
extern pthread_cond_t temperature_cond;
extern long global_tick;

void* tick(void* arg){
    global_tick = 0;
    usleep(PRELOAD_DELAY);

    while (kill_flag == 0){

        usleep(1000);
        pthread_cond_signal(&temperature_cond);
        usleep(19000);

#ifdef DEBUG
        printf("*********** Tick %ld ************** \n", global_tick);
#endif
        LOCK(&tick_mtx);
        global_tick++;
        UNLOCK(&tick_mtx);
        pthread_cond_signal(&tick_cond);

    }
}


long get_tick(){
    return global_tick;
}