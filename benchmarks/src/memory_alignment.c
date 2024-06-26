
#include "benchmark.h"


//#define ARRAY_SIZE 1000000  // Size of array for memory alignment test

void memory_alignment_test(long ARRAY_SIZE) {
    char *array = (char *)malloc(ARRAY_SIZE * sizeof(char));

    // Allocate memory with different alignments
    for (int align = 1; align <= 256; align *= 2) {
        void *ptr;
        posix_memalign(&ptr, align, ARRAY_SIZE * sizeof(char));
        free(ptr);
    }

    free(array);
    printf("Memory alignment test completed for array size %d.\n", ARRAY_SIZE);
}