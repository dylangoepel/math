#include <stdio.h>
#include "../cors.h"

const double dx = 0.001;

int main() {
    for(double x = 0; x < 3; x += dx) {
        printf("%f, %f\n", x, cors_cos(x));
    }
}
