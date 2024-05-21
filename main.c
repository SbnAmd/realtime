//
// Created by sbn on 5/12/24.
//

#include "C/realtime.h"
#include "C/threads/init.h"
#include "Test/perf_count.h"
#include "Test/task_times.h"
#include "Test/task_duration/task_duration.h"
#include "Test/kernel_power/kernel_power_measurement.h"


pthread_cond_t tick_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t temperature_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t tick_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t server_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t temperature_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t manage_to_core_CVes[NUM_CORES] = {PTHREAD_COND_INITIALIZER};
pthread_mutex_t core_mutexes[NUM_CORES] = {PTHREAD_MUTEX_INITIALIZER};

int new_task_IDes[NUM_CORES] = {0};

//Used to notify each core that new task has been assigned
int new_task_stat[NUM_CORES] = {NO_TASK, NO_TASK, NO_TASK, NO_TASK};

//Used to keep core status (Running or Idle)
int core_status[NUM_CORES]={IDLE, IDLE, IDLE, IDLE};

int core_IDes[NUM_CORES];

struct PerformanceEvents perf_event_array[NUM_CORES] = {0};
float temperatures[TOTAL_CORES] = {0};
double power;
unsigned long energy_uj;
int stop_flag = 0;
int kill_flag = 0;
char g_buffer[2048] = {'\0'};
struct timespec start, end;
long elapsed_ns;


int main(){

#ifdef TEST_TASK_DURATION
    check_tasks_duration();
#endif

    init();

//    tasks_time();
//
//    measure_kernel_power();


    return 0;
}

