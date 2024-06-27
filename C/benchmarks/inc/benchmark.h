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



void calculate_primes();
void mm();
void fibonacci();
void factorial();
void quick_sort();
void m_inverse();
void pi();
void rle();
void ackermann();
void m_transpose();
void memory_bandwidth();
void memory_stress();
void memory_access();
void memory_fill();
void cache_miss();
void file_io_benchmark();
void memory_alignment_test();

void simd_vec_add();
void simd_mm();
void simd_dot();
void simd_sse();






#endif //REALTIME_BENCHMARK_H
