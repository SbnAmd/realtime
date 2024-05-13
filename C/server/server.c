//
// Created by sbn on 5/5/24.
//

#include "server.h"


extern pthread_cond_t server_cond;
extern pthread_mutex_t server_mtx;
extern char g_buffer[2048];
extern int stop_flag;

void setup_socket(int* server_fd, struct sockaddr_in* address, int* opt){

    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);

}


void start_server(int* server_fd, struct sockaddr_in* address){


    if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(*server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }


}


void serve(int* server_fd, struct sockaddr_in* address){

    int new_socket, valread;
    char buffer[BUFFER_SIZE] = {0};
    int addrlen = sizeof(address);
    size_t sent_data_bytes;
    size_t length;
#ifdef DEBUG
    struct timespec start, end;
    long elapsed_ns;
#endif





    if ((new_socket = accept(*server_fd, (struct sockaddr *)address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    pthread_cond_signal(&server_cond);


    while (stop_flag == 0) {

        if(pthread_mutex_lock(&server_mtx) != 0){
            printf("Tick locking failed\n");
        }

        pthread_cond_wait(&server_cond, &server_mtx);
#ifdef DEBUG
        clock_gettime(CLOCK_MONOTONIC, &start);
#endif
        // Send JSON data
        length = strlen(g_buffer);
        send(new_socket, &length, sizeof(size_t), 0);

        sent_data_bytes = 0;
        while (sent_data_bytes < strlen(g_buffer))
            sent_data_bytes += send(new_socket, &g_buffer[sent_data_bytes], length-sent_data_bytes, 0);

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);


        // Receive data from client
        valread = read(new_socket, g_buffer, 4);
        if (valread <= 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
#ifdef DEBUG
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("transfer time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds
#endif
        pthread_cond_signal(&server_cond);

        if(pthread_mutex_unlock(&server_mtx) != 0){
            printf("Tick unlocking failed\n");
        }


    }


}

void* run_server(){

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    setup_socket(&server_fd, &address, &opt);
    start_server(&server_fd, &address);
    serve(&server_fd, &address);

}




//int main(){
//
//    struct sched_param params;
//    pthread_attr_t server_thread_attr;
//    pthread_t server_thread;
//
//    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
//    pthread_attr_init(&server_thread_attr);
//    pthread_attr_setinheritsched(&server_thread_attr, PTHREAD_EXPLICIT_SCHED);
//    pthread_attr_setschedpolicy(&server_thread_attr, SCHED_FIFO);
//    pthread_attr_setschedparam(&server_thread_attr, &params);
//
//    if (pthread_create(&server_thread, &server_thread_attr, run_server, NULL) != 0){
//        printf("Thread creation failed\n");
//    }
//    cpu_set_t cpuset;
//    CPU_ZERO(&cpuset);
//    CPU_SET(10, &cpuset);
//    pthread_setaffinity_np(server_thread, sizeof(cpu_set_t), &cpuset);
//
//
//    pthread_join(server_thread, NULL);
//    pthread_attr_destroy(&server_thread_attr);
//
//    return 0;
//
//
//}