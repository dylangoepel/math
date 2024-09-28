#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "../huffman.h"
#include "../deflate.h"

#define BUF_SIZE 256

int main() {
    chf_code chf;

    chf_set_deflate_fixed_lit(&chf);

    uint8_t buf[BUF_SIZE];
    bitbuf bbuf = bitbuf_open(buf, 0);

    uint16_t dec_buf[BUF_SIZE];

    chf_decoder ctx;
    chf_decoder_init(&ctx);
    if(chf_decoder_load(&ctx, &chf) < 0) {
        printf("unable to load huffman tree into decoder\n");
        return 1;
    }

    uint16_t sym_i = 0, byte_i = 0;
    while(1) {
        const int ret = chf_decode(&ctx, &bbuf, dec_buf, BUF_SIZE);
        if(ret == -2) {
            printf("unable to decode data: no match after %d/%d bits: %b\n", ctx.l, chf.n, ctx.c);
            exit(1);
        } else if(ret == -3) {
            printf("unable to decode data: encountered invalid bit sequence: %b\n", ctx.c);
            exit(1);
        } else if(ret == 0) {
            const int n = read(STDIN_FILENO, buf, BUF_SIZE);
            if(n == 0) {
                break;
            }
            printf("[*] read %d bytes\n", n);
            bbuf = bitbuf_open(buf, n);
            byte_i += n;
        } else {
            printf("[%d] ", sym_i);
            for(int i = 0; i < ret; ++i) {
                printf("%d ", dec_buf[i]);
            }
            printf("\n");
            sym_i += ret;
        }
    }

    free(ctx.start);
    printf("[*] decoded %d symbols in %d bytes\n", sym_i, byte_i);
}
