
#include "benchmark.h"


long _factorial(int n) {
    if (n <= 1) return 1;
    return n * _factorial(n - 1);

}


void factorial(int n){

    printf("Factorial completed -->%ldd.\n", _factorial(n));

}

