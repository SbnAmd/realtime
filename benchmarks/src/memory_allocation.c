
#include "benchmark.h"


#define BLOCK_SIZE 100000000  // Size of each memory block

void memory_stress() {
    volatile int count = 0;
    for(int i=0; i < 2000; i++) {
        char *ptr = malloc(BLOCK_SIZE);
        if (ptr == NULL) {
            printf("Failed to allocate memory after %d allocations.\n", count);
            return;
        }
        for(int j = 0; j < 200; j++){
            ptr[j] = (char)count;
        }

        count++;
        free(ptr);
    }
//    printf("%d", count);
}

