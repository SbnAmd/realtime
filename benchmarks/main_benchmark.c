#include "benchmark.h"
#include "perf_monitor/perf_monitor.h"



void(* tasks[19])() = {
        // Memory intensive
        mm,
        fibonacci,
        quick_sort,
        m_inverse,
        pi,
        rle,
        ackermann,
        m_transpose,
        memory_access,               // 55 C
        memory_bandwidth,
        memory_stress,              // 60 C
        memory_fill,                // 57 C
        file_io_benchmark,          // 62 C
        // CPU intensive
        calculate_primes,
        cache_miss,                 // 65 C
        simd_dot,                   // 70 C
        simd_mm,                    // 59 C
        simd_sse,                   // 67 C
        simd_vec_add                // 70 C
};

void perf_to_str(char* buf, long* ptr){
    /* name, core, cycles, instructions, cahce_misses, cache_refs, branch_misses, branch_instructions, page_faults, contexts, migrations, duration, energy, temp*/
    sprintf(buf, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\n",
            ptr[0],
            ptr[1],
            ptr[2],
            ptr[3],
            ptr[4],
            ptr[5],
            ptr[6],
            ptr[7],
            ptr[8],
            ptr[9],
            ptr[10],
            ptr[11],
            ptr[12],
            ptr[13],
            ptr[14],
            ptr[15],
            ptr[16],
            ptr[17],
            ptr[18]
    );
}


void* thread_function(void* arg) {
    int core_id = *((int*)arg);  // Convert the argument back to an integer
    struct timespec start, end;
    long elapsed_ns, duration;
    long task_duration[19];
    char buf[256];
    FILE* fp;

    // Open the file for reading
    fp = fopen("times.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
    }

    SET_CORE(core_id);
    printf("Hello from thread %d\n", core_id);
    for(int j=0; j < 10; j++){
        for(int i = 0; i < 19; i++){

            clock_gettime(CLOCK_MONOTONIC, &start);

            tasks[i]();

            clock_gettime(CLOCK_MONOTONIC, &end);
            // Time calc
            elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
            task_duration[i] = elapsed_ns / 1000000;
        }
        perf_to_str(buf, task_duration);
//        printf("%s\n", buf);
        fputs(buf, fp);
    }

    fclose(fp);
}


int main(){
    int num_threads = 1;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i+12;
        int result = pthread_create(&threads[i], NULL, thread_function, (void*)&thread_ids[i]);
        if (result != 0) {
            printf("Error creating thread %d: %s\n", i, strerror(result));
            return 1;
        }
    }
//    while (1)
//        get_perf_event(15, 100000);
    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }


    return 0;
}