//
// Created by root on 6/4/24.
//
#include "C/realtime.h"


char* fifo_file_names[] = {
        "/core1/tx",
        "/core1/rx",
        "/core2/tx",
        "/core2/rx",
        "/core3/tx",
        "/core3/rx",
        "/core4/tx",
        "/core5/rx",

};


void (*tasks[TASK_COUNT-1])()={&qsort_large, &qsort_small,
                               &bitcnts_large, &bitcnts_small, &basicmath_large,
                               &basicmath_small, &string_search_large, &string_search_small,
                               &fft_large, &fft_small, &crc_large, &crc_small};

void* ex_worker(void* arg) {

    int core_idx = (int)(*((int*)arg));
    const char *fifo_tx = fifo_file_names[core_idx*2];
    const char *fifo_rx = fifo_file_names[core_idx*2 + 1];
    int ret = -1;
    int task_id = 0;


    mkfifo(fifo_tx, 0666);
    mkfifo(fifo_rx, 0666);

    int tx_fd = open(fifo_tx, O_RDWR);
    int rx_fd = open(fifo_rx, O_RDWR);

    // todo: send ready msg to python
    while (1){

        ret = read(rx_fd, (void*)&task_id, sizeof(task_id));
        if (ret <= 0) {
            printf("Core %d failed to receive data\n", core_idx);
            exit(EXIT_FAILURE);
        }
        printf("received task_id : %d\n", task_id);

        if(task_id < 0)
            break;
        else{
            tasks[task_id]();
        }
        ret = 0;
        write(tx_fd, &ret, sizeof(ret));
    }

    close(tx_fd);
    close(rx_fd);
    unlink(fifo_tx); // Remove FIFO file
    unlink(fifo_rx); // Remove FIFO file

    return NULL;
}


int main(){

    struct sched_param params;
    pthread_attr_t ex_attrs[NUM_CORES];
    pthread_t ex_thread[NUM_CORES];

    for(int i = 0; i < NUM_CORES; i++)
        assign_task_to_core(&params, &ex_attrs[i], &ex_thread[i], 12 + i, ex_worker, (void*)&i);

    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(ex_thread[i], NULL);
        pthread_attr_destroy(&ex_attrs[i]);
    }



    return 0;
}