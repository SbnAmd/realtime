//
// Created by sbn on 5/6/24.
//

#include "serdes.h"



void serialize(struct PerformanceEvents *events, char *buffer,
               int num_events, double* power, unsigned long* energy, float * temperatures, int* status) {

    cJSON *root = cJSON_CreateObject();
    cJSON *perf_counts = cJSON_CreateObject();
    cJSON *temperatures_json = cJSON_CreateObject();
    cJSON *perf1 = cJSON_CreateObject();
    cJSON *perf2 = cJSON_CreateObject();
    cJSON *perf3 = cJSON_CreateObject();
    cJSON *perf4 = cJSON_CreateObject();
    char name[32];
    char *json_string;


    cJSON_AddStringToObject(perf1, "name", events[0].name);
    cJSON_AddNumberToObject(perf1, "core_id", events[0].core_idx);
    cJSON_AddNumberToObject(perf1, "cpu_cycles", events[0].cpu_cycles);
    cJSON_AddNumberToObject(perf1, "cpu_instructions", events[0].cpu_instructions);
    cJSON_AddNumberToObject(perf1, "cpu_cache_misses", events[0].cpu_cache_misses);
    cJSON_AddNumberToObject(perf1, "cpu_cache_references", events[0].cpu_cache_references);
    cJSON_AddNumberToObject(perf1, "cpu_branch_misses", events[0].cpu_branch_misses);
    cJSON_AddNumberToObject(perf1, "cpu_branch_instructions", events[0].cpu_branch_instructions);
    cJSON_AddNumberToObject(perf1, "cpu_page_faults", events[0].cpu_page_faults);
    cJSON_AddNumberToObject(perf1, "cpu_context_switches", events[0].cpu_context_switches);
    cJSON_AddNumberToObject(perf1, "cpu_migrations", events[0].cpu_migrations);
    cJSON_AddNumberToObject(perf1, "duration", events[0].duration);
    cJSON_AddNumberToObject(perf1, "status", status[0]);

    cJSON_AddStringToObject(perf2, "name", events[1].name);
    cJSON_AddNumberToObject(perf2, "core_id", events[1].core_idx);
    cJSON_AddNumberToObject(perf2, "cpu_cycles", events[1].cpu_cycles);
    cJSON_AddNumberToObject(perf2, "cpu_instructions", events[1].cpu_instructions);
    cJSON_AddNumberToObject(perf2, "cpu_cache_misses", events[1].cpu_cache_misses);
    cJSON_AddNumberToObject(perf2, "cpu_cache_references", events[1].cpu_cache_references);
    cJSON_AddNumberToObject(perf2, "cpu_branch_misses", events[1].cpu_branch_misses);
    cJSON_AddNumberToObject(perf2, "cpu_branch_instructions", events[1].cpu_branch_instructions);
    cJSON_AddNumberToObject(perf2, "cpu_page_faults", events[1].cpu_page_faults);
    cJSON_AddNumberToObject(perf2, "cpu_context_switches", events[1].cpu_context_switches);
    cJSON_AddNumberToObject(perf2, "cpu_migrations", events[1].cpu_migrations);
    cJSON_AddNumberToObject(perf2, "duration", events[1].duration);
    cJSON_AddNumberToObject(perf2, "status", status[1]);

    cJSON_AddStringToObject(perf3, "name", events[2].name);
    cJSON_AddNumberToObject(perf3, "core_id", events[2].core_idx);
    cJSON_AddNumberToObject(perf3, "cpu_cycles", events[2].cpu_cycles);
    cJSON_AddNumberToObject(perf3, "cpu_instructions", events[2].cpu_instructions);
    cJSON_AddNumberToObject(perf3, "cpu_cache_misses", events[2].cpu_cache_misses);
    cJSON_AddNumberToObject(perf3, "cpu_cache_references", events[2].cpu_cache_references);
    cJSON_AddNumberToObject(perf3, "cpu_branch_misses", events[2].cpu_branch_misses);
    cJSON_AddNumberToObject(perf3, "cpu_branch_instructions", events[2].cpu_branch_instructions);
    cJSON_AddNumberToObject(perf3, "cpu_page_faults", events[2].cpu_page_faults);
    cJSON_AddNumberToObject(perf3, "cpu_context_switches", events[2].cpu_context_switches);
    cJSON_AddNumberToObject(perf3, "cpu_migrations", events[2].cpu_migrations);
    cJSON_AddNumberToObject(perf3, "duration", events[2].duration);
    cJSON_AddNumberToObject(perf3, "status", status[2]);

    cJSON_AddStringToObject(perf4, "name", events[3].name);
    cJSON_AddNumberToObject(perf4, "core_id", events[3].core_idx);
    cJSON_AddNumberToObject(perf4, "cpu_cycles", events[3].cpu_cycles);
    cJSON_AddNumberToObject(perf4, "cpu_instructions", events[3].cpu_instructions);
    cJSON_AddNumberToObject(perf4, "cpu_cache_misses", events[3].cpu_cache_misses);
    cJSON_AddNumberToObject(perf4, "cpu_cache_references", events[3].cpu_cache_references);
    cJSON_AddNumberToObject(perf4, "cpu_branch_misses", events[3].cpu_branch_misses);
    cJSON_AddNumberToObject(perf4, "cpu_branch_instructions", events[3].cpu_branch_instructions);
    cJSON_AddNumberToObject(perf4, "cpu_page_faults", events[3].cpu_page_faults);
    cJSON_AddNumberToObject(perf4, "cpu_context_switches", events[3].cpu_context_switches);
    cJSON_AddNumberToObject(perf4, "cpu_migrations", events[3].cpu_migrations);
    cJSON_AddNumberToObject(perf4, "duration", events[3].duration);
    cJSON_AddNumberToObject(perf4, "status", status[3]);

    for(int i=0; i < TOTAL_CORES; i++){
        sprintf(name,"core%d", i);
        cJSON_AddNumberToObject(temperatures_json, name, temperatures[i]);
    }

    cJSON_AddItemToObject(perf_counts, "core0", perf1);
    cJSON_AddItemToObject(perf_counts, "core1", perf2);
    cJSON_AddItemToObject(perf_counts, "core2", perf3);
    cJSON_AddItemToObject(perf_counts, "core3", perf4);
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

#ifdef DEBUG
    printf("schedule data size = %lu\n", strlen(buffer));
#endif
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
        printf("core0: %d\n", core0->valueint);
        tasks[0] = core0->valueint;
    }

    cJSON *core1 = cJSON_GetObjectItemCaseSensitive(json, "core1");
    if (cJSON_IsNumber(core1)) {
        printf("core1: %d\n", core1->valueint);
        tasks[1] = core1->valueint;
    }

    cJSON *core2 = cJSON_GetObjectItemCaseSensitive(json, "core2");
    if (cJSON_IsNumber(core2)) {
        printf("core2: %d\n", core2->valueint);
        tasks[2] = core2->valueint;
    }

    cJSON *core3 = cJSON_GetObjectItemCaseSensitive(json, "core3");
    if (cJSON_IsNumber(core3)) {
        printf("core3: %d\n", core3->valueint);
        tasks[3] = core3->valueint;
    }

    // Free cJSON object
    cJSON_Delete(json);



}
