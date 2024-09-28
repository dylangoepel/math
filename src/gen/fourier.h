#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/* headers for common functions and constants contained in targets/constants.o,
 * which contains floating point approximations of common constants and functions,
 * see utils/gen_constants.py for details */

#include <complex.h>

extern double __pi;

#define _pi __pi

/* e^(2^(1 - k) * pi * i) for k = 0, ..., 32 */
extern double complex __z32[];

#define ZETA_MAXN 32
#define _zeta(k) __z32[k]

/* e^(2^(-15) * pi * i * k) for k = 0, ..., 2^16 */
extern double complex __f16[];

#define FOURIER_MAXN 16
#define _fourier(n, k) __f16[(k) << (16 - (n))]

#endif
