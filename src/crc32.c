#include <stdint.h>
#include <unistd.h>

#include "gen/crc32.h"

uint8_t u8_rev(uint8_t x) {
    uint8_t r = 0;
    for(int i = 0; i < 8; ++i) {
        if(x & (1 << i)) {
            r |= 1 << (7 - i);
        }
    }
    return r;
}
uint32_t u32_rev(uint32_t x) {
    uint32_t r = 0;
    for(int i = 0; i < 32; ++i) {
        if(x & (1 << i)) {
            r |= 1 << (31 - i);
        }
    }
    return r;
}

uint32_t crc32_update(uint32_t x0, uint8_t *b, uint8_t n) {
    uint32_t rem = x0;
    for(int i = 0; i < n; ++i) {
        const uint32_t c = _crc32_table[(uint8_t) 0xff & rem];
        rem = (rem >> 8) | (((uint32_t)b[i]) << 24);
        rem ^= c;
    }
    return rem;
}
