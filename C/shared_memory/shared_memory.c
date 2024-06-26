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
    int valread, ret = 0;
    size_t length;


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
        // Check if execution finished
        if(kill_flag == 1)
            break;

        length = strlen(g_buffer);
        write(data_fd, g_buffer, length);
        write(data_len_fifo_fd, &length, sizeof(size_t));

        // Clear buffer
        memset(g_buffer, '\0', 2048);

        // Receive data from client
        valread = read(new_schedule_fd, g_buffer, 2048);
        if (valread <= 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        // Check if execution finished
        if(kill_flag == 1)
            break;

        pthread_cond_signal(&server_cond);

        if(pthread_mutex_unlock(&server_mtx) != 0){
            printf("Tick unlocking failed\n");
        }

    }

    // Signal end of execution, double sending 4 bytes because python read 8 bytes
    write(data_len_fifo_fd, &ret, sizeof(ret));
    write(data_len_fifo_fd, &ret, sizeof(ret));
    usleep(1000);
    // Close named pipe (FIFO)
    close(data_len_fifo_fd);
    unlink(fifo_path1); // Remove FIFO file
    close(data_fd);
    unlink(fifo_path2); // Remove FIFO file
    close(new_schedule_fd);
    unlink(fifo_path3); // Remove FIFO file

}