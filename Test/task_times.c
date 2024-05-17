//
// Created by sbn on 5/15/24.
//

#include "task_times.h"

struct sched_param task_time_params;
pthread_attr_t task_time_attr;
pthread_t  task_time_thread;

int stop_1 = 0;

void get_core15_temp(float* core_temperatures) {
    FILE *fp;
    char command[] = "sudo rdmsr -p 15 -u -f 22:16 0x19c";
    char line[MAX_LINE_LENGTH];
    long n;
    int core = 0;

    // Open a pipe to the sensors command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
    }


    // Read and print each line from the command output
    while (fgets(line, sizeof(line), fp) != NULL) {

        n = strtol(line, (char**)NULL, 10);
        *core_temperatures = 100 - n;
        core += 1;
    }


    // Close the pipe
    pclose(fp);


}

void* task_time_worker(){
    struct timespec start, end;
    long elapsed_ns;
    FILE *file;
    float temp = 0;

    file = fopen("task_times.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for(int i = 0 ; i < 300; i++){

        clock_gettime(CLOCK_MONOTONIC, &start);
        qsort_large();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        get_core15_temp(&temp);
        printf("qsort_large time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp); // Elapsed time in milliseconds
        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
        fprintf(file, "%f\n", temp);


        clock_gettime(CLOCK_MONOTONIC, &start);
        qsort_small();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("qsort_small time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
        fprintf(file, "%f\n", temp);



        clock_gettime(CLOCK_MONOTONIC, &start);
        bitcnts_small();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("bitcnts_small time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
        fprintf(file, "%f\n", temp);


        clock_gettime(CLOCK_MONOTONIC, &start);
        bitcnts_large();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("bitcnts_large time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
        fprintf(file, "%f\n", temp);


        clock_gettime(CLOCK_MONOTONIC, &start);
        basicmath_large();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("basicmath_large time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
        fprintf(file, "%f\n", temp);


        clock_gettime(CLOCK_MONOTONIC, &start);
        basicmath_small();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("basicmath_small time : %f,  temperature : %f\n",elapsed_ns / 1000000.0, temp);
        fprintf(file, "%f\n", elapsed_ns / 1000000.0);
        fprintf(file, "%f\n", temp);
    }

    fclose(file);
    stop_1 =1;

}

void tasks_time(){

    assign_task_to_core(&task_time_params, &task_time_attr, &task_time_thread, 15, task_time_worker, NULL);
    pthread_detach(task_time_thread);
    while (stop_1 == 0)
        sleep(10);
}


