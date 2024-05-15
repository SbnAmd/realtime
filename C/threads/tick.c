//
// Created by sbn on 5/12/24.
//

#include "tick.h"

extern pthread_cond_t tick_cond;
extern int kill_flag;

void* tick(void* arg){

    while (kill_flag == 0){

        usleep(20000);

        pthread_cond_signal(&tick_cond);

    }
}