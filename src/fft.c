#include "gen/fourier.h"
#include <complex.h>

void __fft_combine_radix2(double complex d[], int ds, double complex a[], int as, double complex b[], int bs, int n) {
    const int m = 1 << n;
    double complex ax, bx;
    for(int i = 0; i < m; ++i) {
        ax = a[as * i];
        bx = b[bs * i] * _fourier(n + 1, i);
        d[ds * i] = ax + bx;
        d[ds * (m + i)] = ax - bx;
    }
}

void __ifft_combine_radix2(double complex d[], int ds, double complex a[], int as, double complex b[], int bs, int n) {
    const int m = 1 << n, m2 = 1 << n << 1;
    double complex ax, bx;
    for(int i = 0; i < m; ++i) {
        ax = a[as * i];
        bx = b[bs * i] * _fourier(n + 1, m2 - i - 1);
        d[ds * i] = ax + bx;
        d[ds * (m + i)] = ax - bx;
    }
}

void fft_radix2(double complex dat[], int n) {
    double complex *t[2];
    double complex tmp[1 << n];
    t[0] = dat;
    t[1] = tmp;

    int ma = 1 << n;
    for(int ni = 0; ni < n; ++ni) {
        for(int i = 0; i < ma >> 1; ++i) {
            __fft_combine_radix2(&t[(ni+1)%2][i], ma >> 1, &t[ni%2][i], ma, &t[ni%2][i + (ma >> 1)], ma, ni);
        }
        ma >>= 1;
    }
}

void ifft_radix2(double complex dat[], int n) {
    double complex *t[2];
    double complex tmp[1 << n];
    t[0] = dat;
    t[1] = tmp;

    int ma = 1 << n;
    for(int ni = 0; ni < n; ++ni) {
        for(int i = 0; i < ma >> 1; ++i) {
            __ifft_combine_radix2(&t[(ni+1)%2][i], ma >> 1, &t[ni%2][i], ma, &t[ni%2][i + (ma >> 1)], ma, ni);
        }
        ma >>= 1;
    }
}
