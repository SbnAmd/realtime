//
// Created by sbn on 5/5/24.
//

#ifndef REALTIME_SERVER_H
#define REALTIME_SERVER_H

#include "realtime.h"

#define PORT 8080
#define BUFFER_SIZE 1024

struct Data {
    int id;
    float value;
    char name[50];
};
typedef void (*FunctionPtr)();

void setup_socket(int* server_fd, struct sockaddr_in* address, int* opt);

void start_server(int* server_fd, struct sockaddr_in* address);

void serve(int* server_fd, struct sockaddr_in* address);
void* run_server();





#endif //REALTIME_SERVER_H
