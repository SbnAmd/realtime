
#include "benchmark.h"


//#define OBJECT_SIZE 100000000  // Size of each memory object

void memory_stress_large_objects(long OBJECT_SIZE) {
    int count = 0;
    while (1) {
        void *ptr = malloc(OBJECT_SIZE);
        if (ptr == NULL) {
            printf("Failed to allocate memory after %d allocations.\n", count);
            return;
        }
        count++;
        free(ptr);
    }
}