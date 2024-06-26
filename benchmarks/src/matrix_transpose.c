
#include "benchmark.h"


//#define SIZE 1000  // Size of the matrix

void transpose_matrix(double **A, double **transpose, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            transpose[j][i] = A[i][j];
        }
    }
}

void m_transpose(long SIZE) {
    double **A = (double **)malloc(SIZE * sizeof(double *));
    double **transpose = (double **)malloc(SIZE * sizeof(double *));
    for (int i = 0; i < SIZE; i++) {
        A[i] = (double *)malloc(SIZE * sizeof(double));
        transpose[i] = (double *)malloc(SIZE * sizeof(double));
    }

    // Initialize the matrix with random values
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 100;
        }
    }

    // Perform matrix transposition
    transpose_matrix(A, transpose, SIZE);

    // Clean up memory
    for (int i = 0; i < SIZE; i++) {
        free(A[i]);
        free(transpose[i]);
    }
    free(A);
    free(transpose);

    printf("Matrix transposition completed for size %d.\n", SIZE);

}