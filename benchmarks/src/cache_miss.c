
#include "benchmark.h"


#define ARRAY_SIZE 10000000  // Size of array for cache test

void cache_miss() {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        printf("Failed to allocate memory.\n");
        return;
    }

    // Iterate through the array in a non-sequential order
    for (int i = 0; i < ARRAY_SIZE; i += 1024) {
        array[i] = i;
    }

    free(array);
//    printf("Cache miss test completed for array size %d.\n", ARRAY_SIZE);
}