//
// Created by sbn on 5/12/24.
//

#include "manager.h"


void* manager(void* arg){

    int status[NUM_CORES];
    int new_tasks[NUM_CORES];

    // Wait for first schedule
    WAIT_ON_LOCK(&server_mtx, &server_cond);

    while (stop_flag == 0){

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
        serialize(perf_event_array, g_buffer, NUM_CORES, &power, &energy_uj, temperatures);

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

    }

}

