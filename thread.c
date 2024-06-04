//
// Created by root on 6/4/24.
//
#include "C/realtime.h"
#include "C/threads/realtime_t.h"

void* ex_worker(void* arg) {
    extern int kill_flag;
    const char *fifo_path1 = "/tmp/signal_pipe1";
    int task_idx, ret = -1;
//    int core_idx = (int)(*((int*)arg));

    mkfifo(fifo_path1, 0666);

    int data_len_fifo_fd = open(fifo_path1, O_RDWR);


    for (int i = 0; i < 1000; i++){
        write(data_len_fifo_fd, &i, sizeof(i));
    }

    write(data_len_fifo_fd, &ret, sizeof(ret));

    close(data_len_fifo_fd);
    unlink(fifo_path1); // Remove FIFO file

    return NULL;
}


int main(){

    struct sched_param params;
    pthread_attr_t ex_attrs;
    pthread_t ex_thread;

    assign_task_to_core(&params, &ex_attrs, &ex_thread, 12, ex_worker, NULL);

    pthread_join(ex_thread, NULL);
    pthread_attr_destroy(&ex_attrs);


    return 0;
}