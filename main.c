//
// Created by sbn on 5/12/24.
//

#include "TaskPerformance/task_performance.h"
#include "C/utils/utils.h"



int main(){
    struct sched_param params;
    pthread_attr_t t_attr;
    pthread_t t_thread;

    assign_task_to_core(&params, &t_attr, &t_thread, TEST_CORE, run_test, NULL);
    pthread_join(t_thread, NULL);
    pthread_attr_destroy(&t_attr);
    return 0;
}

