#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../crc32.h"

#define BUFSIZE 1024
int main() {
    uint8_t b[BUFSIZE];
    uint32_t crc = 0xffffffff;
    int n;

    while(1) {
        n = read(STDIN_FILENO, b, BUFSIZE);
        if(n <= 0) {
            break;
        }
        crc = crc32_update(crc, b, n);
    }
    if(n < 0) {
        return 1;
    }

    crc ^= 0xffffffff;
    
    printf("%x %b\n", crc, crc);

    return 0;
}
