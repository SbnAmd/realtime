//
// Created by sbn on 5/17/24.
//

#include "task_duration.h"

#define TEST_REPEAT  20

void (*tasks[TASK_COUNT-1])()={&qsort_large, &qsort_small,
                                    &bitcnts_large, &bitcnts_small, &basicmath_large,
                                    &basicmath_small, &string_search_large, &string_search_small,
                                    &fft_large, &fft_small, &crc_large, &crc_small};

char* task_names[] = { "qsort_large", "qsort_small", "bitcnts_large", "bitcnts_small",
                       "basicmath_large", "basicmath_small", "string_search_large", "string_search_small",
                       "fft_large", "fft_small", "crc_large", "crc_small",

};

char* task_file_names[] = { "/home/sbn/Desktop/realtime/Data/duration/qsort_large.txt",
                            "/home/sbn/Desktop/realtime/Data/duration/qsort_small.txt",
                            "/home/sbn/Desktop/realtime/Data/duration/bitcnts_large.txt",
                            "/home/sbn/Desktop/realtime/Data/duration/bitcnts_small.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/basicmath_large.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/basicmath_small.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/string_search_large.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/string_search_small.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/fft_large.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/fft_small.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/crc_large.txt",
                           "/home/sbn/Desktop/realtime/Data/duration/crc_small.txt",
};


void* tasks_duration_worker(){

    struct timespec start, end;
    long elapsed_ns;
    double task_duration[TEST_REPEAT][13];
    FILE* fp;
    char buf[32];

    for(int j = 0 ; j < TEST_REPEAT ; j++){
        for(int t = 0 ; t < TASK_COUNT-1 ; t++){

            clock_gettime(CLOCK_MONOTONIC, &start);
            tasks[t]();
            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            task_duration[j][t] = elapsed_ns / 1000000.0;
        }
    }

    for(int t = 0 ; t < TASK_COUNT-1 ; t++){
        fp = fopen(task_file_names[t], "w");
        if (fp != NULL){
            for(int j=0; j < TEST_REPEAT; j++){
                sprintf(buf, "%f\n", task_duration[j][t]);
                fputs(buf, fp);
            }
        }
        fclose(fp);
    }


}


void check_tasks_duration(){

    struct sched_param params;
    pthread_attr_t attr;
    pthread_t thread;

    assign_task_to_core(&params, &attr, &thread, 4, tasks_duration_worker, NULL);
    pthread_join(thread, NULL);

};