
#include "conf.h"
#include "thread.h"

int main(){

    struct sched_param params;
    pthread_attr_t ex_attrs[NUM_CORES];
    pthread_t ex_thread[NUM_CORES];
    int core_id[NUM_CORES]={0, 1, 2, 3};

    for(int i = 0; i < NUM_CORES; i++){
        int result = pthread_create(&ex_thread[i], NULL, ex_worker, (void*)&core_id[i]);
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
