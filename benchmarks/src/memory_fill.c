
#include "benchmark.h"


//#define ARRAY_SIZE 100000000  // Size of array for memory fill

void memory_fill(long ARRAY_SIZE) {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        printf("Failed to allocate memory.\n");
        return;
    }

    // Fill the array with a pattern
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i % 256;
    }

    free(array);
    printf("Memory fill test completed for array size %d.\n", ARRAY_SIZE);
}