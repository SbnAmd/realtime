//
// Created by root on 6/26/24.
//

#include "benchmark.h"




void calculate_primes(long limit) {
    int count = 0;
    for (int num = 2; num <= limit; num++) {
        bool is_prime = true;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            count++;
        }
    }
    printf("Found %d prime numbers up to %ld.\n", count, limit);
}
