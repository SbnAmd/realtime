
#include "benchmark.h"


#define SIZE 300  // Size of the matrix

void invert_matrix(double **A, double **inverse, int size) {
    // Initialize the identity matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            inverse[i][j] = (i == j) ? 1 : 0;
        }
    }

    // Gaussian elimination
    for (int i = 0; i < size; i++) {
        double pivot = A[i][i];
        for (int j = 0; j < size; j++) {
            A[i][j] /= pivot;
            inverse[i][j] /= pivot;
        }
        for (int k = 0; k < size; k++) {
            if (k != i) {
                double factor = A[k][i];
                for (int j = 0; j < size; j++) {
                    A[k][j] -= factor * A[i][j];
                    inverse[k][j] -= factor * inverse[i][j];
                }
            }
        }
    }
}

void m_inverse() {
    double **A = (double **)malloc(SIZE * sizeof(double *));
    double **inverse = (double **)malloc(SIZE * sizeof(double *));
    for (int i = 0; i < SIZE; i++) {
        A[i] = (double *)malloc(SIZE * sizeof(double));
        inverse[i] = (double *)malloc(SIZE * sizeof(double));
    }

    // Initialize the matrix with random values
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 100;
        }
    }

    // Perform matrix inversion
    invert_matrix(A, inverse, SIZE);

    // Clean up memory
    for (int i = 0; i < SIZE; i++) {
        free(A[i]);
        free(inverse[i]);
    }
    free(A);
    free(inverse);

//    printf("Matrix inversion completed for size %d.\n", SIZE);

}