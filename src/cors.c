#include "gen/fourier.h"
#include <complex.h>
#include <math.h>

#define CORS_ITERATION_COUNT 32

// computes cos(2pi * x) for 0 <= x <= 1
double cors_cos(double x) {
    double complex p = 1;
    double rem = fmod(x, 1);

    if(rem >= 0.5) {
        p = -1;
        rem -= 0.5;
    }

    if(rem >= 0.25) {
        p *= -1;
        rem = 0.5 - rem;
    }

    rem = 2 * rem;

    for(int i = 2; i < CORS_ITERATION_COUNT; ++i) {
        if(rem >= 0.5) {
            p *= _zeta(i);
            rem -= 0.5;
        }
        rem = rem * 2;
    }
    return p;
}
