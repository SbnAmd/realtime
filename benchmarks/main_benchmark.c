#include "benchmark.h"



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


void* thread_function(void* arg) {
    int core_id = *((int*)arg);  // Convert the argument back to an integer
    SET_CORE(core_id);
    printf("Hello from thread %d\n", core_id);
    while (1){
        tasks[core_id-6]();
    }
}


int main(){
    int num_threads = 4;
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

    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }


    return 0;
}