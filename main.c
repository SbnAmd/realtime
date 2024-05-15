//
// Created by sbn on 5/12/24.
//

#include "C/realtime.h"
#include "C/threads/init.h"
#include "Test/perf_count.h"


pthread_cond_t tick_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t tick_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t server_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t manage_to_core_CVes[NUM_CORES] = {PTHREAD_COND_INITIALIZER};

pthread_mutex_t core_mutexes[NUM_CORES] = {PTHREAD_MUTEX_INITIALIZER};
int new_task_IDes[NUM_CORES] = {0};
int new_task_stat[NUM_CORES] = {0};
int core_status[NUM_CORES]={IDLE};
int core_IDes[NUM_CORES];
struct PerformanceEvents perf_event_array[NUM_CORES] = {0};
float temperatures[TOTAL_CORES] = {0};
double power;
unsigned long energy_uj;
int stop_flag = 0;
int kill_flag = 0;
char g_buffer[2048] = {'\0'};

void test(){

    printf("perf test\n");
}

int main(){

    init();
//    perf_count_test(test, perf_event_array, 8);

    return 0;
}

