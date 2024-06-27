
#include "benchmark.h"

#define N 50
long _factorial(int n) {
    if (n <= 1) return 1;
    return n * _factorial(n - 1);

}


void factorial(){

//    printf("Factorial completed -->%ldd.\n", _factorial(N));

}

