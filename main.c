//
// Created by sbn on 5/12/24.
//

#include "TaskPerformance/task_performance.h"
#include "C/utils/utils.h"

#define DATA_SAMPLES       15000

int stop_flag=0;
int run = 0;
int core_stat[4] = {0};

char* cur_frequency_file_path[] = {
        "/sys/devices/system/cpu/cpu12/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu13/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu14/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu15/cpufreq/scaling_cur_freq",
};

char* cur_temp_file_path[] = {
        "/sys/class/hwmon/hwmon3/temp6_input",
        "/sys/class/hwmon/hwmon3/temp7_input",
        "/sys/class/hwmon/hwmon3/temp8_input",
        "/sys/class/hwmon/hwmon3/temp9_input"
};


unsigned long read_var(char* temp_path){

    FILE *fp;

    char buffer[MAX_BUF_SIZE];

    // Open the file for reading
    fp = fopen(temp_path, "r");
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


void* record_information(){

    char l_buffer[512] = {'\0'};
    unsigned long temps[4];
    unsigned long freqs[4];
    unsigned long energy;
    FILE* fp;

    struct timespec start, end;
    long elapsed_ns;
    int tmp_running_task_id[4], tmp_core_stat[4];
    extern int running_task_id[4];
    extern pthread_mutex_t cores_mtx[4];

//    fp = fopen("/home/sbn/Desktop/realtime/Data/Perf/multicore.txt", "w");
//    if(fp == NULL){
//        printf("File did not open\n");
//        exit(1);
//    }
//
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    for(int i = 0; i < DATA_SAMPLES; i++){
//        if(DATA_SAMPLES - i < 1000){
//            set_freq(0, i%4);
//            set_freq(1, i%4);
//            set_freq(2, i%4);
//            set_freq(3, i%4);
//        }else{
//            set_freq(0, -1);
//            set_freq(1, -1);
//            set_freq(2, -1);
//            set_freq(3, -1);
//        }
//
//        usleep(50000);
        temps[0] = read_var(cur_temp_file_path[0]);
        printf("%lu\n", temps[0]);
//        temps[1] = read_var(cur_temp_file_path[1]);
//        temps[2] = read_var(cur_temp_file_path[2]);
//        temps[3] = read_var(cur_temp_file_path[3]);
//
//        freqs[0] = read_var(cur_frequency_file_path[0]);
//        freqs[1] = read_var(cur_frequency_file_path[1]);
//        freqs[2] = read_var(cur_frequency_file_path[2]);
//        freqs[3] = read_var(cur_frequency_file_path[3]);
//
//        energy = get_core_power();
//
//        for(int k = 0; k < 4; k++){
//            LOCK(&cores_mtx[k]);
//            tmp_running_task_id[k] = running_task_id[k];
//            tmp_core_stat[k] = core_stat[k];
//            UNLOCK(&cores_mtx[k]);
//        }
//
//        sprintf(l_buffer, "%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %d, %d, %d, %d, %d, %d, %d, %d, %lu\n",
//                temps[0],
//                temps[1],
//                temps[2],
//                temps[3],
//                freqs[0],
//                freqs[1],
//                freqs[2],
//                freqs[3],
//                tmp_core_stat[0],
//                tmp_core_stat[1],
//                tmp_core_stat[2],
//                tmp_core_stat[3],
//                tmp_running_task_id[0],
//                tmp_running_task_id[1],
//                tmp_running_task_id[2],
//                tmp_running_task_id[3],
//                energy
//        );
//        fputs(l_buffer, fp);
//
//
//
//    }
//
//    fclose(fp);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
//    printf("Duration : %f ms\n",elapsed_ns / 1000000.0);


}


int main(){
    struct sched_param params;
    pthread_attr_t t_attr[5];
    pthread_t t_thread[5];
    unsigned long tmp;

    assign_task_to_core(&params, &t_attr[0], &t_thread[0], CORE_1, core_worker, 0);
//    assign_task_to_core(&params, &t_attr[1], &t_thread[1], CORE_2, core_worker, (int *) 1);
//    assign_task_to_core(&params, &t_attr[2], &t_thread[2], CORE_3, core_worker, (int *) 2);
//    assign_task_to_core(&params, &t_attr[3], &t_thread[3], CORE_4, core_worker, (int *) 3);
//    assign_task_to_core(&params, &t_attr[4], &t_thread[4], 10, record_information, NULL);

//    pthread_join(t_thread[4], NULL);
    for(int k = 0;; k++){
        run = 1;
        usleep(1000000);
        tmp = read_var(cur_temp_file_path[0]);
        printf("%lu\n", tmp);
//        run = 0;
//        sleep(1);
        set_freq(0, k%3);
        tmp = read_var(cur_temp_file_path[0]);
        printf("%lu\n", tmp);
    }
    stop_flag = 1;
    pthread_join(t_thread[0], NULL);
//    pthread_join(t_thread[1], NULL);
//    pthread_join(t_thread[2], NULL);
//    pthread_join(t_thread[3], NULL);
    pthread_attr_destroy(&t_attr[0]);
//    pthread_attr_destroy(&t_attr[1]);
//    pthread_attr_destroy(&t_attr[2]);
//    pthread_attr_destroy(&t_attr[3]);
//    pthread_attr_destroy(&t_attr[4]);
    return 0;
}

