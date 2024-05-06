//
// Created by sbn on 5/5/24.
//

#include "server.h"




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
    struct Data data;
    data.id = 10;
    data.value = 3.14f;
    strcpy(data.name, "John Doe");
    int addrlen = sizeof(address);
    long cnt;
    struct timespec start, end;
    long elapsed_ns;

    while (1) {

//        printf("Waiting for connection...\n");
        if ((new_socket = accept(*server_fd, (struct sockaddr *)address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        clock_gettime(CLOCK_MONOTONIC, &start);
        // Serialize struct to JSON
        sprintf(buffer, "{\"id\": %d, \"value\": %f, \"name\": \"%s\"}", data.id, data.value, data.name);

        // Send JSON data
        send(new_socket, buffer, strlen(buffer), 0);
//        printf("Data sent\n");

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data from client
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
//        printf("Received: %s\n", buffer);
        clock_gettime(CLOCK_MONOTONIC, &end);

        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        double elapsed_ms = elapsed_ns / 1000000.0; // Elapsed time in milliseconds
        printf("Elapsed time : %.2f milliseconds\n", elapsed_ms);
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