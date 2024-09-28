#include <stdio.h>
#include <complex.h>
#include "../fft.h"

int main() {
    double complex ts[1024];
    double dx = 1.0 / 512;

    for(int i = 0; i < 1024; ++i) {
        ts[i] = (dx * (i - 512)) * (dx * (i - 512));
    }

    fft_radix2(ts, 10);
    ifft_radix2(ts, 10);

    for(int i = 0; i < 1024; ++i) {
        printf("%f,%f\n", dx * (i-512), creal(ts[i]));
    }
}
