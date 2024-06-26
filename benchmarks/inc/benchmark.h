//
// Created by root on 6/26/24.
//

#ifndef REALTIME_BENCHMARK_H
#define REALTIME_BENCHMARK_H
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <xmmintrin.h>  // Header for SSE intrinsics
#include <immintrin.h>  // Header for AVX intrinsics

#define SET_CORE(core_id) \
    do { \
        cpu_set_t cpuset; \
        CPU_ZERO(&cpuset); \
        CPU_SET(core_id, &cpuset); \
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); \
    } while (0)



void calculate_primes(long limit);
void mm(long SIZE);
void fibonacci(int n);
void factorial(int n);
void quick_sort(long ARRAY_SIZE);
void m_inverse(long SIZE);
void pi(long NUM_POINTS);
void rle(long INPUT_SIZE);
void ackermann(int m, int n);
void m_transpose(long SIZE);
void memory_bandwidth(long ARRAY_SIZE);
void memory_stress(long BLOCK_SIZE);
void memory_access(long ARRAY_SIZE);
void memory_fill(long ARRAY_SIZE);
void cache_miss(long ARRAY_SIZE);
void file_io_benchmark(long FILE_SIZE);
void memory_alignment_test(long ARRAY_SIZE);

void simd_vec_add(long VECTOR_SIZE);
void simd_mm(long N);
void simd_dot(long VECTOR_SIZE);
void simd_sse(long WIDTH, long HEIGHT);






#endif //REALTIME_BENCHMARK_H
