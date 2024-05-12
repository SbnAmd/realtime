//
// Created by sbn on 5/8/24.
//

#include "utils.h"


void assign_task_to_core(struct sched_param* params, pthread_attr_t* attr, pthread_t* thread, int core, void* worker, int* arg) {

    params->sched_priority = sched_get_priority_max(SCHED_FIFO);
    pthread_attr_init(attr);
    pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(attr, SCHED_FIFO);
    pthread_attr_setschedparam(attr, params);

    int ret = pthread_create(thread, attr, worker, arg);

    if ( ret != 0){
        printf("Thread creation failed with code %d\n", ret);
    }
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    pthread_setaffinity_np(*thread, sizeof(cpu_set_t), &cpuset);

}
