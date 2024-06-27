
#include "benchmark.h"


#define ARRAY_SIZE 5000000  // Size of arrays for memory copy

void memory_bandwidth() {
    double *src = (double *)malloc(ARRAY_SIZE * sizeof(double));
    double *dst = (double *)malloc(ARRAY_SIZE * sizeof(double));

    // Initialize the source array with some values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        src[i] = (double)i;
    }

    // Perform memory copy from src to dst
    memcpy(dst, src, ARRAY_SIZE * sizeof(double));

    free(src);
    free(dst);
//    printf("Memory bandwidth test completed for array size %d.\n", ARRAY_SIZE);
}

