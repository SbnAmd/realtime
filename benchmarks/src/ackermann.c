
#include "benchmark.h"

int _ackermann(int m, int n) {
    if (m == 0) {
        return n + 1;
    } else if (m > 0 && n == 0) {
        return _ackermann(m - 1, 1);
    } else {
        return _ackermann(m - 1, _ackermann(m, n - 1));
    }
}

void ackermann() {
    int m = 3, n = 6;  // Adjust parameters for more intensive computation
//    printf("Ackermann(%d, %d) = %d\n", m, n, _ackermann(m, n));

}