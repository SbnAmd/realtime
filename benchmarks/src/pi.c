
#include "benchmark.h"

//#define NUM_POINTS 10000000

double calculate_pi(long NUM_POINTS) {
    int inside_circle = 0;
    for (int i = 0; i < NUM_POINTS; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            inside_circle++;
        }
    }
    return 4.0 * inside_circle / (double )NUM_POINTS;
}

void pi(long NUM_POINTS) {
    srand(time(NULL));  // Seed the random number generator
    double pi = calculate_pi(NUM_POINTS);
    printf("Calculated value of Pi is approximately %f.\n", pi);
}