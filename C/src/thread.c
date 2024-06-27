//
// Created by root on 6/4/24.
//
#include "thread.h"



char* fifo_file_names[] = {
        "/tmp/core1_tx",
        "/tmp/core1_rx",
        "/tmp/core2_tx",
        "/tmp/core2_rx",
        "/tmp/core3_tx",
        "/tmp/core3_rx",
        "/tmp/core4_tx",
        "/tmp/core4_rx",

};


extern void(* tasks[19])();


void init(const char *fifo_tx,const char *fifo_rx, int* tx_fd, int* rx_fd){
    mkfifo(fifo_tx, 0666);
    mkfifo(fifo_rx, 0666);

    *tx_fd = open(fifo_tx, O_WRONLY);
    *rx_fd = open(fifo_rx, O_RDONLY );

    if (*rx_fd == -1) {
        perror("Failed to open FIFO");
        exit(1);
    }
}

void de_init(const char *fifo_tx,const char *fifo_rx, int* tx_fd, int* rx_fd){
    close(*tx_fd);
    close(*rx_fd);
    unlink(fifo_tx); // Remove FIFO file
    unlink(fifo_rx); // Remove FIFO file
}

inline void serialize(char* buf, long duration){
    sprintf(buf, "%ld", duration);
}


inline long time_task(int task_id){
    struct timespec task_start, task_end;
    long elapsed_ns;
    clock_gettime(CLOCK_MONOTONIC, &task_start);

    tasks[task_id]();

    clock_gettime(CLOCK_MONOTONIC, &task_start);
    elapsed_ns = (task_end.tv_sec - task_start.tv_sec) * 1000000000 + (task_end.tv_nsec - task_start.tv_nsec);
    return elapsed_ns/1000000;
}

inline int receive(char* buff, int* rx_fd){
    ssize_t bytes_read = 0;

    while (bytes_read < 16){
        bytes_read += read(*rx_fd, (void*)buff, 16);
    }
    char a = buff[0];
    char b = buff[1];
    return (a - 48)*10 + (b - 48);
}

inline void send(char* buf,int* tx_fd){

    size_t length = strlen(buf);
    write(*tx_fd, &length, sizeof(length));
    write(*tx_fd, buf, strlen(buf));
}

void* ex_worker(void* arg) {

    int core_idx = (int)(*((int*)arg));
    const char *fifo_tx = fifo_file_names[core_idx*2];
    const char *fifo_rx = fifo_file_names[core_idx*2 + 1];
    int tx_fd, rx_fd;
    char recv_buff[16] = {'\0'};
    char send_buff[16] = {'\0'};
    int task_id = 0;
    long duration = 0;

    init(fifo_tx, fifo_rx, &tx_fd, &rx_fd);

    while (1){
        task_id = receive(recv_buff, &rx_fd);
        if(task_id < 0){
            break;
        }

//        printf("Core[%d], Task[%d] started, issue tick = %s \n",core_idx, task_id, &recv_buff[2]);
        duration = time_task(task_id);
        serialize(send_buff, duration);
        send(send_buff, &tx_fd);
//        printf("\t\tCore[%d], Task[%d] Finished, issue tick = %s \n",core_idx, task_id, &recv_buff[2]);
    }

    de_init(fifo_tx, fifo_rx, &tx_fd, &rx_fd);
    return NULL;
}


