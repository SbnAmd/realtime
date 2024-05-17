//
// Created by sbn on 5/15/24.
//

#include "task_times.h"

#define MAX_ELEMENTS   1000

struct sched_param task_time_params;
pthread_attr_t task_time_attr1, task_time_attr2, task_time_attr3, task_time_attr4;
pthread_t  task_time_thread1, task_time_thread2, task_time_thread3, task_time_thread4;

int stop_1 = 0;

char command_temp15[] = "rdmsr -p 15 -u -f 22:16 0x19c";
char command_temp14[] = "sudo rdmsr -p 14 -u -f 22:16 0x19c";
char command_temp13[] = "sudo rdmsr -p 13 -u -f 22:16 0x19c";
char command_temp12[] = "sudo rdmsr -p 12 -u -f 22:16 0x19c";

void get_core_temp(float* core_temperatures, char* command) {
    FILE *fp;

    char line[MAX_LINE_LENGTH];
    long n;
    int core = 0;

    // Open a pipe to the sensors command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        *core_temperatures = -1;
    } else{
        // Read and print each line from the command output
        while (fgets(line, sizeof(line), fp) != NULL) {

            n = strtol(line, (char**)NULL, 10);
            *core_temperatures = 100 - n;
            core += 1;
        }

        // Close the pipe
        pclose(fp);
    }





}


void get_core15_temp(float* core_temperatures){
    get_core_temp(core_temperatures, command_temp15);
}

void get_core14_temp(float* core_temperatures){
    get_core_temp(core_temperatures, command_temp14);
}

void get_core13_temp(float* core_temperatures){
    get_core_temp(core_temperatures, command_temp13);
}

void get_core12_temp(float* core_temperatures){
    get_core_temp(core_temperatures, command_temp12);
}


void task_time_worker(char* file_name, char* command_temp){
    struct timespec start, end;
    long elapsed_ns;
    FILE *file;
    float temperature;
    float temp[MAX_ELEMENTS] = {0};
    float time_p[MAX_ELEMENTS] = {0};

//    file = fopen("task_temp15.txt", "w");


    for(int i = 0 ; i < MAX_ELEMENTS;i++){

//        clock_gettime(CLOCK_MONOTONIC, &start);
//        qsort_large();
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

//        printf("qsort_large time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temperature); // Elapsed time in milliseconds
//        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
//        fprintf(file, "%f\n", temp);



//        clock_gettime(CLOCK_MONOTONIC, &start);
//        qsort_small();
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
//        printf("qsort_small time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
//        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
//        fprintf(file, "%f\n", temp);



//        clock_gettime(CLOCK_MONOTONIC, &start);
        bitcnts_small();
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
//        printf("bitcnts_small time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
//        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
//        fprintf(file, "%f\n", temp);


//        clock_gettime(CLOCK_MONOTONIC, &start);
        bitcnts_large();
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
//        printf("bitcnts_large time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
//        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
//        fprintf(file, "%f\n", temp);


//        clock_gettime(CLOCK_MONOTONIC, &start);
        basicmath_large();
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
//        printf("basicmath_large time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
//        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
//        fprintf(file, "%f\n", temp);


//        clock_gettime(CLOCK_MONOTONIC, &start);
        basicmath_small();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec) * 1000000000 + (end.tv_nsec);
//        printf("basicmath_small time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
//        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
//        fprintf(file, "%f\n", temp);

        get_core_temp(&temperature, command_temp);
        printf("temp[%d] = %f \n",i, temperature);
        temp[i] = temperature;
        time_p[i] = (float )elapsed_ns / 1000000.0;


    }

    file = fopen(file_name, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for(int i=0; i < MAX_ELEMENTS ; i++){
        fprintf(file, "%f\n", temp[i]);
        fprintf(file, "%f\n", time_p[i]);
    }


    fclose(file);
    stop_1 =1;

}

void task_worker(char* file_name, char* command_temp){
    for(int i = 0 ; i < MAX_ELEMENTS; i++){

//        qsort_large();
//
//        qsort_small();

        bitcnts_small();

        bitcnts_large();

        basicmath_large();

        basicmath_small();

    }
}

void* core15_worker(){
    char filename[] = {"task_temp15.txt"};
    task_time_worker(filename,command_temp15);
}

void* core14_worker(){
    char filename[] = {"task_temp14.txt"};
    task_time_worker(filename,command_temp14);
}

void* core13_worker(){
    char filename[] = {"task_temp13.txt"};
    task_time_worker(filename, command_temp13);
}

void* core12_worker(){
    char filename[] = {"task_temp12.txt"};
    task_time_worker(filename, command_temp12);
}


void tasks_time(){

    assign_task_to_core(&task_time_params, &task_time_attr1, &task_time_thread1, 15, core15_worker, NULL);
    assign_task_to_core(&task_time_params, &task_time_attr2, &task_time_thread2, 14, task_worker, NULL);
    assign_task_to_core(&task_time_params, &task_time_attr3, &task_time_thread3, 13, task_worker, NULL);
//    assign_task_to_core(&task_time_params, &task_time_attr4, &task_time_thread4, 12, task_worker, NULL);
    pthread_detach(task_time_thread1);
    pthread_detach(task_time_thread2);
    pthread_detach(task_time_thread3);
//    pthread_detach(task_time_thread4);
    while (stop_1 == 0)
        sleep(10);
}


