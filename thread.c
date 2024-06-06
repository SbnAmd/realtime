//
// Created by root on 6/4/24.
//
#include "C/realtime.h"


char* fifo_file_names[] = {
        "/tmp/core1_tx",
        "/tmp/core1_rx",
        "/tmp/core2_tx",
        "/tmp/core2_rx",
        "/tmp/core3_tx",
        "/tmp/core3_rx",
        "/tmp/core4_tx",
        "/tmp/core5_rx",

};


void (*raw_tasks[TASK_COUNT-1])()={&qsort_large, &qsort_small,
                               &bitcnts_large, &bitcnts_small, &basicmath_large,
                               &basicmath_small, &string_search_large, &string_search_small,
                               &fft_large, &fft_small, &crc_large, &crc_small};

void serialize(struct PerformanceEvents *events, cJSON *root){

    cJSON_AddNumberToObject(root, "cpu_cycles", events->cpu_cycles);
    cJSON_AddNumberToObject(root, "cpu_instructions", events->cpu_instructions);
    cJSON_AddNumberToObject(root, "cpu_cache_misses", events->cpu_cache_misses);
    cJSON_AddNumberToObject(root, "cpu_cache_references", events->cpu_cache_references);
    cJSON_AddNumberToObject(root, "cpu_branch_misses", events->cpu_branch_misses);
    cJSON_AddNumberToObject(root, "cpu_branch_instructions", events->cpu_branch_instructions);
    cJSON_AddNumberToObject(root, "cpu_page_faults", events->cpu_page_faults);
    cJSON_AddNumberToObject(root, "cpu_context_switches", events->cpu_context_switches);
    cJSON_AddNumberToObject(root, "cpu_migrations", events->cpu_migrations);
    cJSON_AddNumberToObject(root, "duration", events->duration);
}


void* ex_worker(void* arg) {

    int core_idx = (int)(*((int*)arg));
    const char *fifo_tx = fifo_file_names[core_idx*2];
    const char *fifo_rx = fifo_file_names[core_idx*2 + 1];
    struct PerformanceEvents perf_event;
    cJSON *root = cJSON_CreateObject();
    char *json_string;
    size_t length;
    int ret = -1;
    int task_id = 0;


    mkfifo(fifo_tx, 0666);
    mkfifo(fifo_rx, 0666);

    int tx_fd = open(fifo_tx, O_RDWR);
    int rx_fd = open(fifo_rx, O_RDWR);

    printf("Core %d waiting for new schedule\n", core_idx);

    while (1){

        ret = read(rx_fd, (void*)&task_id, 1);
        if (ret <= 0) {
            printf("Core %d failed to receive data\n", core_idx);
            exit(EXIT_FAILURE);
        }

        task_id -= 48;
        if(task_id < 0)
            break;
        else{
            printf("Core %d received task_id : %d, %x\n",core_idx, task_id, task_id);
            run_task_and_get_perf_event(raw_tasks[task_id], &perf_event, core_idx);
            serialize(&perf_event, root);
        }
        json_string = cJSON_PrintUnformatted(root);
        length = strlen(json_string);
        write(tx_fd, &length, sizeof(length));
        write(tx_fd, json_string, strlen(json_string));
    }

    close(tx_fd);
    close(rx_fd);
    unlink(fifo_tx); // Remove FIFO file
    unlink(fifo_rx); // Remove FIFO file

    return NULL;
}


int main(){

    struct sched_param params;
    pthread_attr_t ex_attrs[NUM_CORES];
    pthread_t ex_thread[NUM_CORES];
    int core_id[NUM_CORES]={0, 1, 2, 3};

    for(int i = 0; i < NUM_CORES; i++)
        assign_task_to_core(&params, &ex_attrs[i], &ex_thread[i], 12 + i, ex_worker, &core_id[i]);

    for(int i = 0; i < NUM_CORES; i++){
        pthread_join(ex_thread[i], NULL);
        pthread_attr_destroy(&ex_attrs[i]);
    }



    return 0;
}