//
// Created by sbn on 5/12/24.
//

#ifndef REALTIME_TICK_H
#define REALTIME_TICK_H

#include "realtime.h"


void* tick(void* arg){

    while (stop_flag == 0){

        usleep(20000);

        pthread_cond_signal(&tick_cond);

    }
}



#endif //REALTIME_TICK_H
