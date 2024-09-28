#include "../gen/fourier.h"
#include <complex.h>
#include <stdio.h>

int main() {
    double dx = 1 / 1024.0, x = 0;
    for(int i = 0; i < 1024; ++i) {
        printf("%f,%f\n", x, creal(_fourier(10, i)));
        x += dx;
    }
}
