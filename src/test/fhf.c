#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "../huffman.h"
#include "../deflate.h"


int main() {
    chf_code chf;

    chf_set_deflate_fixed_lit(&chf);

    chf_encoder encoder;
    if(chf_encoder_init(&encoder, &chf) < 0) {
        fprintf(stderr, "error initializing huffman encoder\n");
        exit(1);
    }

    uint16_t in[500];
    for(int i = 0; i < 500; ++i) {
        in[i] = i % 288;
    }

    uint8_t out[1024];
    memset(out, 0, 1024);

    bitbuf outbuf = bitbuf_open(out, 1024);

    const int ret = chf_encode(&encoder, &outbuf, in, 500);
    if(ret < 0) {
        fprintf(stderr, "unable to encode data\n");
        exit(1);
    }

    int k = 0;
    while(k < ret) {
        k += write(STDOUT_FILENO, out + k, 1024 - outbuf.len + 1 - k);
    }

    chf_encoder_free(&encoder);
}
