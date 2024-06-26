
#include "benchmark.h"

//#define ARRAY_SIZE 100000000  // Size of array for memory access

void memory_access(long ARRAY_SIZE) {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        printf("Failed to allocate memory.\n");
        return;
    }

    // Access every element in the array
    long long sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
        sum += array[i];
    }

    free(array);
    printf("Memory access test completed for array size %d with sum %lld.\n", ARRAY_SIZE, sum);
}