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

void extract_tasks(int* tasks_list, char* recv_buffer){
    char c1, c2;
    for(int k =0 ; k < 8; k++){
        char c = recv_buffer[k];
        printf("gbuff[%d] = %d\n", k, atoi(&c));
    }
    for(int j=0; j < NUM_CORES * 2; j+=2){
        int recv_task_id;

        c1 = (char)recv_buffer[j];
//        c1 = (char) "0";
        c2 = recv_buffer[j+1];
        recv_task_id=  atoi(&c1)*10 + atoi(&c2);
#ifdef DEBUG
        printf("%d - %d\n", atoi(&c1) ,atoi(&c2));
        printf("%c - %c\n",c1 ,c2);
#endif
        tasks_list[j/2] = recv_task_id;
    }
}
