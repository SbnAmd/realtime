
#include "conf.h"
#include "thread.h"

int main(){

    struct sched_param params;
    pthread_attr_t ex_attrs[NUM_CORES];
    pthread_t ex_thread[NUM_CORES];
    int core_id[NUM_CORES]={0, 1, 2, 3};

    for(int i = 0; i < NUM_CORES; i++){
        params.sched_priority = sched_get_priority_max(SCHED_FIFO);
        pthread_attr_init(&ex_attrs[i]);
        pthread_attr_setinheritsched(&ex_attrs[i], PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&ex_attrs[i], SCHED_FIFO);
        pthread_attr_setschedparam(&ex_attrs[i], &params);

        int result = pthread_create(&ex_thread[i], &ex_attrs[i], ex_worker, (void*)&core_id[i]);
        if (result != 0) {
            printf("Error creating thread %d: %s\n", i, strerror(result));
            return 1;
        }
    }

    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(ex_thread[i], NULL);
        printf("Core %d joined\n", i);
        pthread_attr_destroy(&ex_attrs[i]);
    }

    return 0;
}
