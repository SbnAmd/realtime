//
// Created by sbn on 5/8/24.
//

#include "shared_memory.h"

extern int stop_flag;

extern pthread_cond_t server_cond;
extern pthread_mutex_t server_mtx;
extern char g_buffer[2048];
extern int kill_flag;


void* shared_mem_worker() {
    int valread;
    size_t length;

#ifdef DEBUG
    struct timespec start, end;
    long elapsed_ns;
#endif



    // Create named pipe (FIFO) for signaling
    const char *fifo_path1 = "/tmp/signal_pipe1";
    const char *fifo_path2 = "/tmp/signal_pipe2";
    const char *fifo_path3 = "/tmp/signal_pipe3";
    mkfifo(fifo_path1, 0666);
    mkfifo(fifo_path2, 0666);
    mkfifo(fifo_path3, 0666);

    int data_len_fifo_fd = open(fifo_path1, O_RDWR);
    int data_fd = open(fifo_path2, O_RDWR);
    int new_schedule_fd = open(fifo_path3, O_RDWR);

    pthread_cond_signal(&server_cond);

    // Main loop to continuously write data
    while (kill_flag == 0) {

        if(pthread_mutex_lock(&server_mtx) != 0){
            printf("Tick locking failed\n");
        }

        pthread_cond_wait(&server_cond, &server_mtx);

#ifdef DEBUG
        clock_gettime(CLOCK_MONOTONIC, &start);
#endif

        length = strlen(g_buffer);
        write(data_fd, g_buffer, length);
//#ifdef DEBUG
//        printf("sending status\n");
//#endif
        write(data_len_fifo_fd, &length, sizeof(size_t));

        // Clear buffer
        memset(g_buffer, '\0', 2048);


        // Receive data from client
        valread = read(new_schedule_fd, g_buffer, 2048);
        if (valread <= 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
//#ifdef DEBUG
//        printf("received new schedule\n");
//#endif

//#ifdef DEBUG
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
//        printf("shared mem transfer time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds
//#endif
        pthread_cond_signal(&server_cond);

        if(pthread_mutex_unlock(&server_mtx) != 0){
            printf("Tick unlocking failed\n");
        }

    }

    // Close named pipe (FIFO)
    close(data_len_fifo_fd);
    unlink(fifo_path1); // Remove FIFO file
    close(data_fd);
    unlink(fifo_path2); // Remove FIFO file
    close(new_schedule_fd);
    unlink(fifo_path3); // Remove FIFO file

}