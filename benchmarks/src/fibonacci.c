
#include "benchmark.h"


#define N 60


long long _fibonacci(long n) {
    if (n <= 1) return n;
    long long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}


void fibonacci() {
    for(int i = 0 ; i < 32000; i++){
        _fibonacci(N);
//        printf("Fibo %ld\n", p);
    }

}