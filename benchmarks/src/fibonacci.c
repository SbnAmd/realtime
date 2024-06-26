
#include "benchmark.h"


#define N 50


void _fibonacci(int n) {
    if (n <= 1) return;
    long long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
//    printf("Fibonacci completed for size %lld.\n", b);
}


void fibonacci() {
    _fibonacci(N);
}