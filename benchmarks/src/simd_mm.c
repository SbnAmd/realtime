
#include "benchmark.h"


#define N 256  // Size of the matrix

void avx_matrix_multiplication(float *A, float *B, float *C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            __m256 c = _mm256_setzero_ps();
            for (int k = 0; k < n; k += 8) {
                __m256 a = _mm256_loadu_ps(&A[i * n + k]);
                __m256 b = _mm256_loadu_ps(&B[k * n + j]);
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
            }
            float sum[8];
            _mm256_storeu_ps(sum, c);
            C[i * n + j] = sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];
        }
    }
}

void simd_mm() {
    float *A = (float *)_mm_malloc(N * N * sizeof(float), 32);
    float *B = (float *)_mm_malloc(N * N * sizeof(float), 32);
    float *C = (float *)_mm_malloc(N * N * sizeof(float), 32);

    // Initialize matrices with some values
    for (int i = 0; i < N * N; i++) {
        A[i] = i * 0.5f;
        B[i] = i * 2.0f;
        C[i] = 0.0f;
    }

    // Perform SIMD matrix multiplication
    avx_matrix_multiplication(A, B, C, N);

//    printf("AVX matrix multiplication completed.\n");

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

}
