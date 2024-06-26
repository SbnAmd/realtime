
#include "benchmark.h"



//#define VECTOR_SIZE 1024 * 1024  // Size of the vectors

float avx_dot_product(float *a, float *b, int size) {
    __m256 sum = _mm256_setzero_ps();
    for (int i = 0; i < size; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        sum = _mm256_add_ps(sum, _mm256_mul_ps(va, vb));
    }

    // Horizontal add to accumulate the sum
    float result[8];
    _mm256_storeu_ps(result, sum);
    return result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7];
}

void simd_dot(long VECTOR_SIZE) {
    float *a = (float *)_mm_malloc(VECTOR_SIZE * sizeof(float), 32);
    float *b = (float *)_mm_malloc(VECTOR_SIZE * sizeof(float), 32);

    // Initialize vectors with some values
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = i * 0.5f;
        b[i] = i * 2.0f;
    }

    // Perform SIMD dot product
    float dot_product = avx_dot_product(a, b, VECTOR_SIZE);

    printf("AVX dot product completed: %f\n", dot_product);

    _mm_free(a);
    _mm_free(b);

}
