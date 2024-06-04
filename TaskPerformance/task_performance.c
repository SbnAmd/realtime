//
// Created by root on 5/31/24.
//

#include "task_performance.h"

extern int stop_flag;
extern int core_stat[4];
int freq_selection[4] = {0, 1, 2, 3};
int freq_counter[4] = {7, 1, 2, 6};
int running_task_id[4] = {0,0,0,0};
pthread_mutex_t cores_mtx[4] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER};
struct PerformanceEvents perf_event_array[TEST_REPETITION] = {0};
unsigned long energy_arr[TEST_REPETITION] = {0};
unsigned long temp_arr[TEST_REPETITION] = {0};
void (*tasks[TASK_COUNT])(int)={&QsortLargeTask, &QsortSmallTask,
                               &BitCountLargeTask, &BitCountSmallTask, &BasicMathLargeTask,
                               &BasicMathSmallTask, &StringSearchLargeTask, &StringSearchSmallTask,
                               &FFTLargeTask, &FFTSmallTask, &CRCLargeTask, &CRCSmallTask};

void (*raw_tasks[TASK_COUNT])()={&qsort_large, &qsort_small,
                               &bitcnts_large, &bitcnts_small, &basicmath_large,
                               &basicmath_small, &string_search_large, &string_search_small,
                               &fft_large, &fft_small, &crc_large, &crc_small};

char* task_file_names[] = { "/home/sbn/Desktop/realtime/Data/Perf/qsort_large.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/qsort_small.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/bitcnts_large.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/bitcnts_small.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/basicmath_large.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/basicmath_small.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/string_search_large.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/string_search_small.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/fft_large.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/fft_small.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/crc_large.txt",
                            "/home/sbn/Desktop/realtime/Data/Perf/crc_small.txt",
};

char* frequency_file_path[] = {
                            "/sys/devices/system/cpu/cpu12/cpufreq/scaling_min_freq",
                            "/sys/devices/system/cpu/cpu12/cpufreq/scaling_max_freq",
                            "/sys/devices/system/cpu/cpu13/cpufreq/scaling_min_freq",
                            "/sys/devices/system/cpu/cpu13/cpufreq/scaling_max_freq",
                            "/sys/devices/system/cpu/cpu14/cpufreq/scaling_min_freq",
                            "/sys/devices/system/cpu/cpu14/cpufreq/scaling_max_freq",
                            "/sys/devices/system/cpu/cpu15/cpufreq/scaling_min_freq",
                            "/sys/devices/system/cpu/cpu15/cpufreq/scaling_max_freq",
};

char* frequencies[] = {
        "1000000",
        "2500000",
        "4000000",
        "4000000",
        "4700000",
};

void task(FunctionPtr real_task, int core_idx, char *name){

    struct PerformanceEvents* perf_event = &perf_event_array[core_idx];
    strcpy(perf_event->name, name);
    perf_event->start = 0;
    run_task_and_get_perf_event(real_task, perf_event, core_idx);
    perf_event->end = 0;
}

void perf_to_str(char* buf, struct PerformanceEvents* perf, unsigned long* energy, unsigned long* temperature){
    /* name, core, cycles, instructions, cahce_misses, cache_refs, branch_misses, branch_instructions, page_faults, contexts, migrations, duration, energy, temp*/
    sprintf(buf, "%s, %d, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %f, %lld, %lld\n",
            perf->name,
            perf->core_idx,
            perf->cpu_cycles,
            perf->cpu_instructions,
            perf->cpu_cache_misses,
            perf->cpu_cache_references,
            perf->cpu_branch_misses,
            perf->cpu_branch_instructions,
            perf->cpu_page_faults,
            perf->cpu_context_switches,
            perf->cpu_migrations,
            perf->duration,
            *energy,
            *temperature
            );
}

unsigned long get_core_power(){

    FILE *fp;

    char buffer[MAX_BUF_SIZE];

    // Open the file for reading
    fp = fopen(POWER_PATH, "r");
    if (fp == NULL) {
        perror("Error opening file");
    }

    // Read the contents of the file
    if (fgets(buffer, MAX_BUF_SIZE, fp) == NULL) {
        perror("Error reading file");
        fclose(fp);
    }

    // Close the file
    fclose(fp);

    // Convert the string to an integer (energy in microjoules)
    return strtoul(buffer, NULL, 10);
}

unsigned long get_core_temp(){

    FILE *fp;

    char buffer[MAX_BUF_SIZE];

    // Open the file for reading
    fp = fopen(CORE_TEMP_PATH, "r");
    if (fp == NULL) {
        perror("Error opening file");
    }

    // Read the contents of the file
    if (fgets(buffer, MAX_BUF_SIZE, fp) == NULL) {
        perror("Error reading file");
        fclose(fp);
    }

    // Close the file
    fclose(fp);

    // Convert the string to an integer (energy in microjoules)
    return strtoul(buffer, NULL, 10);
}


void QsortLargeTask(int core_idx){
    static char name[] = {"QsortLargeTask"};
    task(qsort_large, core_idx, name);
}

void QsortSmallTask(int core_idx){
    static char name[] = {"QsortSmallTask"};
    task(qsort_small ,core_idx, name);
}

void BitCountLargeTask(int core_idx){
    static char name[] = {"BitCountLargeTask"};
    task(bitcnts_large ,core_idx, name);
}

void BitCountSmallTask(int core_idx){
    static char name[] = {"BitCountSmallTask"};
    task(bitcnts_small ,core_idx, name);
}

void BasicMathLargeTask(int core_idx){
    static char name[] = {"BasicMathLargeTask"};
    task(basicmath_large ,core_idx, name);
}

void BasicMathSmallTask(int core_idx){
    static char name[] = {"BasicMathSmallTask"};
    task(basicmath_small    ,core_idx, name);
}

void StringSearchLargeTask(int core_idx){
    static char name[] = {"StringSearchLargeTask"};
    task(string_search_large, core_idx, name);
}

void StringSearchSmallTask(int core_idx){
    static char name[] = {"StringSearchSmallTask"};
    task(string_search_small, core_idx, name);
}

void FFTLargeTask(int core_idx){
    static char name[] = {"FFTLargeTask"};
    task(fft_large, core_idx, name);
}

void FFTSmallTask(int core_idx){
    static char name[] = {"FFTSmallTask"};
    task(fft_small, core_idx, name);
}

void CRCLargeTask(int core_idx){
    static char name[] = {"CRCLargeTask"};
    task(crc_large, core_idx, name);
}

void CRCSmallTask(int core_idx){
    static char name[] = {"CRCSmallTask"};
    task(crc_small, core_idx, name);
}


void* run_test(){
    char l_buffer[512] = {'\0'};
    FILE* fp;
    // For warming up
    for(int i =0; i < 2000; i++)
        qsort_large();

    printf("Begin Test.....\n");
    for(int t = 0; t < TASK_COUNT; t++){
        printf("Test task id %d\n", t);
        for(int i = 0; i < TEST_REPETITION; i++){
            if(i % 100 == 0)
                printf("Round %d\n", i);
            tasks[t](i);
            energy_arr[i] = get_core_power();
            temp_arr[i] = get_core_temp();
        }
        fp = fopen(task_file_names[t], "w");
        if(fp == NULL){
            printf("File did not open\n");
            exit(1);
        }

        for(int i = 0; i < TEST_REPETITION; i++){
            perf_to_str(l_buffer, &perf_event_array[i], &energy_arr[i], &temp_arr[i]);
            fputs(l_buffer, fp);
        }

        fclose(fp);
    }
}

void write_file(char* _filepath, char* freq){

    FILE *file = fopen(_filepath, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    if (fprintf(file, "%s", freq) < 0) {
        perror("Error writing to file");
        fclose(file);
        exit(1);
    }

    // Close the file
    if (fclose(file) != 0) {
        perror("Error closing file");
        exit(1);
    }

}


void idle(){
    srandom(time(NULL));
//    int random_sleep = random() % 4;

    usleep(10*1000);

}

void set_freq(int core_id,  int force){
    unsigned char buf[1];
    int next_freq;
    if(force < 0){
        if(RAND_bytes(buf, sizeof(buf)) != 1){
            exit(1);
        }
        next_freq = buf[0] % 4;
    } else{
        next_freq = force;
    }


    write_file(frequency_file_path[2*core_id], frequencies[next_freq]);
    write_file(frequency_file_path[2*core_id+1], frequencies[next_freq]);
}

void* core_worker(int core_id){

    unsigned char buf[1];
    extern int run;
    printf("Core %d begin\n", core_id);

    while (stop_flag == 0){
//        set_freq(core_id);
        if(RAND_bytes(buf, sizeof(buf)) != 1){
            exit(1);
        }
//        srandom(core_id+1);
//        int random_task = buf[0] % 13;
        int random_task = 5;

//        if(random_task != 12){
        if(run > 0){
//            LOCK(&cores_mtx[core_id]);
//            core_stat[core_id] = 1;
//            running_task_id[core_id] = random_task;
//            UNLOCK(&cores_mtx[core_id]);
            raw_tasks[random_task]();
        } else{
//            LOCK(&cores_mtx[core_id]);
//            core_stat[core_id] = 0;
//            running_task_id[core_id] = random_task;
//            UNLOCK(&cores_mtx[core_id]);
            idle();
        }


    }
}





