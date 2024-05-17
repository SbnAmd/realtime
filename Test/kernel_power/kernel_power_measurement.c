//
// Created by sbn on 5/16/24.
//

#include "kernel_power_measurement.h"

int stop = 0;
struct sched_param _params;
pthread_attr_t _attrs[SLEEPING_CORES];
pthread_t _threads[SLEEPING_CORES];

void* sleep_function(){

    while (stop == 0){
        usleep(SLEEP_TIME);
    }
}

void* stress_function(){

    long n =0 ;
    long cnt = 100000000000 ;

    while (cnt-->0){
        n++;
    }
}


void measure_kernel_power(){
    FILE *fp;
    char buffer[32];
    unsigned long long energy1 ,energy2;
    double power;

    // Put other cores to sleep
    for(int i = 0; i <= END_CORE-BEGIN_CORE; i++){
        assign_task_to_core(&_params, &_attrs[i], &_threads[i], i, sleep_function, NULL);
//        assign_task_to_core(&_params, &_attrs[i], &_threads[i], i+CORE_OFFSET, stress_function, NULL);
    }

    // Open the file for reading


    energy1 = 0;
    for(int i = 0 ; i < REPETITION; i++){

        fp = fopen(RAPL_PATH, "r");
        if (fp == NULL) {
            perror("Error opening file\n");
            exit(1);
        }

        // Read the contents of the file
        if (fgets(buffer, 32, fp) == NULL) {
            perror("Error reading file");
            continue;
        }

        // Convert the string to an integer (energy in microjoules)
        energy2 = strtoul(buffer, NULL, 10);

        if(energy1 != 0){
            power = (double )(energy2 - energy1)/MEASURE_DURATION;
            printf("Power =  %f W\n", power);
        }

        energy1 = energy2;

        // Close the file
        fclose(fp);
        usleep(MEASURE_DURATION);
    }


    for(int i = 0; i <= END_CORE-BEGIN_CORE; i++){
        pthread_detach(_threads[i]);
    }
    stop = 1;

}