//
// Created by sbn on 5/12/24.
//

#include "manager.h"

extern pthread_mutex_t server_mtx;
extern pthread_cond_t server_cond;

extern pthread_mutex_t core_mutexes[NUM_CORES];
extern pthread_mutex_t temperature_mtx;
//Used to keep core status (Running or Idle)
extern int core_status[NUM_CORES];
extern float temperatures[TOTAL_CORES];
extern double power;
extern unsigned long energy_uj;
extern struct PerformanceEvents perf_event_array[NUM_CORES];
extern char g_buffer[2048];
extern int new_task_IDes[NUM_CORES];
extern int new_task_stat[NUM_CORES];
extern int stop_flag;
extern pthread_cond_t tick_cond;
extern pthread_mutex_t tick_mtx;
extern pthread_cond_t manage_to_core_CVes[NUM_CORES];
extern int kill_flag;
extern struct timespec start;

void copy_cores_status(int* status){
    printf("Core status : \n");
    for(int i = 0; i < NUM_CORES; i++){

        // Copying core statuses
        LOCK(&core_mutexes[i]);
        status[i] = core_status[i];
#ifdef DEBUG
        if(status[i] == IDLE){
            printf("\tCore[%d] is IDLE\n",i);
        }else{
            printf("\tCore[%d] is RUNNING\n",i);
        }
#endif
        UNLOCK(&core_mutexes[i]);
    }

}

void dispatch_tasks(int* _core_status, int* new_tasks){

    for(int i = 0; i < NUM_CORES; i++) {
        if (_core_status[i] == IDLE && new_tasks[i] != -1) {
            if(new_tasks[i] > 0){
                LOCK(&core_mutexes[i]);

                new_task_IDes[i] = new_tasks[i];
                new_task_stat[i] = NEW_TASK;
                pthread_cond_signal(&manage_to_core_CVes[i]);
                UNLOCK(&core_mutexes[i]);
            }else{
                LOCK(&core_mutexes[i]);
                new_task_stat[i] = NO_TASK;
                pthread_cond_signal(&manage_to_core_CVes[i]);
                UNLOCK(&core_mutexes[i]);
            }

        }
    }
}


void* manager(void* arg){
    int bool = 0;
    int _core_status[NUM_CORES];
    int new_tasks[NUM_CORES] = {1, 2, 3,4};

#ifdef DEBUG
    struct timespec lend, lstart;
    long elapsed_ns;
#endif


    // Wait for communication to get ready
    WAIT_ON_LOCK(&server_mtx, &server_cond);


    while (kill_flag == 0){

        // wait until tick signal
        WAIT_ON_LOCK(&tick_mtx, &tick_cond);

        // Copy cores status (IDLE or RUNNING) to local var
        copy_cores_status(_core_status);

        LOCK(&temperature_mtx);
        // Get total cpu power and energy
        get_power_and_energy(&power, &energy_uj);
        UNLOCK(&temperature_mtx);

        // Serialize and send data to python scheduler
        serialize(perf_event_array, g_buffer, NUM_CORES, &power, &energy_uj, temperatures, _core_status);

        // Send signal to scheduler to get data
        pthread_cond_signal(&server_cond);

        // Wait until new schedule
        WAIT_ON_LOCK(&server_mtx, &server_cond);


        // Deserialize
        deserialize(new_tasks, g_buffer);
        if(bool==0){
            clock_gettime(CLOCK_MONOTONIC, &start);
            bool=1;
        }


        // Dispatch tasks
        dispatch_tasks(_core_status, new_tasks);



        if(stop_flag == 1){
            kill_flag = 1;
            pthread_cond_signal(&server_cond);
            break;
        }
    }
}

