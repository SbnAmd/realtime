//
// Created by sbn on 5/8/24.
//

#ifndef REALTIME_UTILS_H
#define REALTIME_UTILS_H

#include "C/realtime.h"


#define WAIT_ON_LOCK(mutex, condition) \
    do { \
        if (pthread_mutex_lock(mutex) != 0) { \
            printf("Tick locking failed\n"); \
        } \
        pthread_cond_wait(condition, mutex); \
        if (pthread_mutex_unlock(mutex) != 0) { \
            printf("Tick unlocking failed\n"); \
        } \
    } while(0)

#define LOCK(mutex) \
    do { \
        if (pthread_mutex_lock(mutex) != 0) { \
            printf("Locking failed\n"); \
        } \
    } while(0)

#define UNLOCK(mutex) \
    do { \
        if (pthread_mutex_unlock(mutex) != 0) { \
            printf("Locking failed\n"); \
        } \
    } while(0)

void assign_task_to_core(struct sched_param* params, pthread_attr_t* attr, pthread_t* thread, int core, void* worker, int* arg);

void extract_tasks(int* tasks_list, char* recv_buffer);

#endif //REALTIME_UTILS_H
