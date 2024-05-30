//
// Created by sbn on 5/6/24.
//

#include "serdes.h"



void serialize(struct PerformanceEvents *events, char *buffer,
               int num_events, double* power, unsigned long* energy, float * temperatures, int* status) {

    cJSON *root = cJSON_CreateObject();
    cJSON *perf_counts = cJSON_CreateObject();
    cJSON *temperatures_json = cJSON_CreateObject();
    cJSON *perf_arr[NUM_CORES] = {cJSON_CreateObject(), cJSON_CreateObject(), cJSON_CreateObject(), cJSON_CreateObject()};
    char name[32];
    char *json_string;

    for(int j = 0 ; j < NUM_CORES ; j++){
        if(strcmp(events[j].name, "QsortSmallTask") == 0){
            printf("ds***** tick = %ld, stat = %d, core = %d, cycles = %ld\n", get_tick(), status[j], events[j].core_idx, events[j].cpu_cycles );
        }
        cJSON_AddStringToObject(perf_arr[j], "name", (status[j] == IDLE) ? events[j].name : "Null");
        cJSON_AddNumberToObject(perf_arr[j], "core_id", (status[j] == IDLE) ? events[j].core_idx : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_cycles", (status[j] == IDLE) ? (double)events[j].cpu_cycles : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_instructions", (status[j] == IDLE) ? (double)events[j].cpu_instructions : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_cache_misses", (status[j] == IDLE) ? (double)events[j].cpu_cache_misses : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_cache_references", (status[j] == IDLE) ? (double)events[j].cpu_cache_references : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_branch_misses", (status[j] == IDLE) ? (double)events[j].cpu_branch_misses : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_branch_instructions", (status[j] == IDLE) ? (double)events[j].cpu_branch_instructions : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_page_faults", (status[j] == IDLE) ? (double)events[j].cpu_page_faults : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_context_switches", (status[j] == IDLE) ? (double)events[j].cpu_context_switches : -1);
        cJSON_AddNumberToObject(perf_arr[j], "cpu_migrations", (status[j] == IDLE) ? (double)events[j].cpu_migrations : -1);
        cJSON_AddNumberToObject(perf_arr[j], "duration", (status[j] == IDLE) ? (double)events[j].duration : -1);
        cJSON_AddNumberToObject(perf_arr[j], "start_tick", (status[j] == IDLE) ? (double)events[j].start : -1);
        cJSON_AddNumberToObject(perf_arr[j], "end_tick", (status[j] == IDLE) ? (double)events[j].end : -1);
        cJSON_AddNumberToObject(perf_arr[j], "status", status[j]);

        if(status[j] == IDLE){
            memset(&events[j], 0, sizeof(struct PerformanceEvents));
            memset(&events[j].name, '\0', MAX_LENGTH);
        }
    }


    for(int i=0; i < TOTAL_CORES; i++){
        sprintf(name,"core%d", i);
        cJSON_AddNumberToObject(temperatures_json, name, temperatures[i]);
    }

    cJSON_AddItemToObject(perf_counts, "core0", perf_arr[0]);
    cJSON_AddItemToObject(perf_counts, "core1", perf_arr[1]);
    cJSON_AddItemToObject(perf_counts, "core2", perf_arr[2]);
    cJSON_AddItemToObject(perf_counts, "core3", perf_arr[3]);
    cJSON_AddItemToObject(root, "performance_counters", perf_counts);
    cJSON_AddItemToObject(root, "temperatures", temperatures_json);
    cJSON_AddNumberToObject(root, "power", *power);
    cJSON_AddNumberToObject(root, "energy", *energy);

    json_string = cJSON_PrintUnformatted(root);
    memcpy(buffer, json_string, strlen(json_string));
    buffer[strlen(json_string)] = '\0';

    cJSON_Delete(root);
    free(json_string);


}


void deserialize(int* tasks, char* buffer){

//#ifdef DEBUG
//    printf("schedule data size = %lu\n", strlen(buffer));
//#endif
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }

    // Extract each value
    cJSON *core0 = cJSON_GetObjectItemCaseSensitive(json, "core0");
    if (cJSON_IsNumber(core0)) {
//        printf("core0: %d\n", core0->valueint);
        tasks[0] = core0->valueint;
    }

    cJSON *core1 = cJSON_GetObjectItemCaseSensitive(json, "core1");
    if (cJSON_IsNumber(core1)) {
//        printf("core1: %d\n", core1->valueint);
        tasks[1] = core1->valueint;
    }

    cJSON *core2 = cJSON_GetObjectItemCaseSensitive(json, "core2");
    if (cJSON_IsNumber(core2)) {
//        printf("core2: %d\n", core2->valueint);
        tasks[2] = core2->valueint;
    }

    cJSON *core3 = cJSON_GetObjectItemCaseSensitive(json, "core3");
    if (cJSON_IsNumber(core3)) {
//        printf("core3: %d\n", core3->valueint);
        tasks[3] = core3->valueint;
    }

    // Free cJSON object
    cJSON_Delete(json);



}
