
#include "benchmark.h"

//#define ARRAY_SIZE 1000000  // Array size for sorting

void quicksort(int *array, int low, long high) {
    if (low < high) {
        int pivot = array[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (array[j] <= pivot) {
                i++;
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        int temp = array[i + 1];
        array[i + 1] = array[high];
        array[high] = temp;

        quicksort(array, low, i);
        quicksort(array, i + 2, high);
    }
}

void quick_sort(long ARRAY_SIZE) {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));

    // Fill the array with random values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 1000000;
    }

    // Perform quicksort on the array
    quicksort(array, 0, ARRAY_SIZE - 1);

    free(array);
    printf("Quicksort completed for an array size of %d.\n", ARRAY_SIZE);
}