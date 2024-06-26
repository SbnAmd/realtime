
#include "benchmark.h"


//#define VECTOR_SIZE 1024 * 1024 // Size of the vectors

void sse_vector_addition(float *a, float *b, float *result, int size) {
    for (int i = 0; i < size; i += 4) {
        // Load 4 floats from each array
        __m128 va = _mm_loadu_ps(&a[i]);
        __m128 vb = _mm_loadu_ps(&b[i]);

        // Add the vectors
        __m128 vresult = _mm_add_ps(va, vb);

        // Store the result
        _mm_storeu_ps(&result[i], vresult);
    }
}

void simd_vec_add(long VECTOR_SIZE) {
    float *a = (float *)_mm_malloc(VECTOR_SIZE * sizeof(float), 16);
    float *b = (float *)_mm_malloc(VECTOR_SIZE * sizeof(float), 16);
    float *result = (float *)_mm_malloc(VECTOR_SIZE * sizeof(float), 16);

    // Initialize vectors with some values
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = i * 0.5f;
        b[i] = i * 2.0f;
    }

    // Perform SIMD vector addition
    sse_vector_addition(a, b, result, VECTOR_SIZE);

    printf("SSE vector addition completed.\n");

    _mm_free(a);
    _mm_free(b);
    _mm_free(result);


}
