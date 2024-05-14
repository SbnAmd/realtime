//
// Created by sbn on 5/12/24.
//

#include "manager.h"

extern pthread_mutex_t server_mtx;
extern pthread_cond_t server_cond;
extern pthread_mutex_t core_mutexes[NUM_CORES];
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


void* manager(void* arg){

    int status[NUM_CORES];
    int new_tasks[NUM_CORES];

    // Wait for first schedule
    WAIT_ON_LOCK(&server_mtx, &server_cond);

    while (kill_flag == 0){

        for(int i = 0; i < NUM_CORES; i++){

            // Copying core statuses
            LOCK(&core_mutexes[i]);
            status[i] = core_status[i];
            UNLOCK(&core_mutexes[i]);
        }

#ifdef DEBUG
        struct timespec start, end;
        long elapsed_ns;

        clock_gettime(CLOCK_MONOTONIC, &start);
#endif

        // Get each core's temp
        get_core_temperatures(temperatures);

#ifdef DEBUG
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        printf("temp get time : %f\n",elapsed_ns / 1000000.0); // Elapsed time in milliseconds
#endif


        // Get total cpu power and energy
        get_power_and_energy(&power, &energy_uj);

        // Serialize to send to scheduler
        serialize(perf_event_array, g_buffer, NUM_CORES, &power, &energy_uj, temperatures, status);

        // Send signal to server to get data
        pthread_cond_signal(&server_cond);

        // Wait until new schedule
        WAIT_ON_LOCK(&server_mtx, &server_cond);

        // Deserialize
        // fixme: if there is more than 99 tasks, if
        extract_tasks(new_tasks, g_buffer);


        // Dispatch tasks

        for(int i = 0; i < NUM_CORES; i++) {
            if (status[i] == IDLE) {
                LOCK(&core_mutexes[i]);

                new_task_IDes[i] = new_tasks[i];
                new_task_stat[i] = 1;
                pthread_cond_signal(&manage_to_core_CVes[i]);
                UNLOCK(&core_mutexes[i]);
            }
        }

        WAIT_ON_LOCK(&tick_mtx, &tick_cond);

        if(stop_flag == 1){
            kill_flag = 1;
            for(int i = 0; i < NUM_CORES; i++){
                UNLOCK(&core_mutexes[i]);
                pthread_cond_signal(&manage_to_core_CVes[i]);
            }
#ifdef DEBUG
            printf("break, kill flag = %d\n", kill_flag);
#endif
            for(int i = 0; i < NUM_CORES; i++){
                UNLOCK(&core_mutexes[i]);
                pthread_cond_signal(&manage_to_core_CVes[i]);
            }
            break;
        }

    }


}

