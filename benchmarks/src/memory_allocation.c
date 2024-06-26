
#include "benchmark.h"


#define BLOCK_SIZE 1000000  // Size of each memory block

void memory_stress() {
    int count = 0;
    while (1) {
        void *ptr = malloc(BLOCK_SIZE);
        if (ptr == NULL) {
//            printf("Failed to allocate memory after %d allocations.\n", count);
            return;
        }
        count++;
        free(ptr);
    }
}

