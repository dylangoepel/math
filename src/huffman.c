#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "bitbuf.h"
#include "huffman.h"

static inline int __chf_decoder_gen_start_codes(chf_decoder *decoder) {
    decoder->start = malloc(sizeof(uint32_t) * decoder->chf->n);
    if(decoder->start == NULL) {
        return -1;
    }
    uint32_t code = 0;
    uint8_t l = 1;
    for(int i = 0; i<decoder->chf->positions[decoder->chf->n - 1]; ++i) {
        while(decoder->chf->positions[l - 1] <= i) {
            code <<= 1;
            decoder->start[l - 1] = code;
            ++l;
        }
        ++code;
    }
    return 0;
}

void chf_decoder_init(chf_decoder *decoder) {
    memset(decoder, (uint8_t)0, sizeof(chf_decoder));
}

int chf_decoder_load(chf_decoder *decoder, chf_code *chf) {
    decoder->chf = chf;
    return __chf_decoder_gen_start_codes(decoder);
}

int chf_decode_bit(chf_decoder *decoder, uint8_t bit) {
    decoder->c <<= 1;
    if(bit) {
        decoder->c |= 1;
    }
    ++decoder->l;

    const uint32_t start_code = decoder->l == 1 ? 0 : decoder->start[decoder->l - 2];
    if(decoder->c < start_code) {
        return -2;
    }

    const uint16_t start_index = decoder->l == 1 ? 0 : decoder->chf->positions[decoder->l - 2];
    const uint16_t index = decoder->c - start_code + start_index;
    if(index < decoder->chf->positions[decoder->l - 1]) {
        decoder->c = decoder->l = 0;
        return decoder->chf->symbols[index];
    }

    return decoder->l == decoder->chf->n ? -2 : -1;
}

int chf_decode_symbol(chf_decoder *decoder, bitbuf *buf) {
    int ret = -1;
    while(ret == -1) {
        const int r = bitbuf_read(buf);
        if(r < 0) {
            return r;
        }

        ret = chf_decode_bit(decoder, r & 1);
    }
    return ret;
}

int chf_decode(chf_decoder *decoder, bitbuf *b, uint16_t buf[], uint16_t n) {
    uint16_t i = 0;
    while(i < n) {
        const int ret = chf_decode_symbol(decoder, b);
        if(ret < -1) {
            return ret;
        } else if(ret == -1) {
            return i;
        }

        buf[i] = (uint16_t) ret;
        ++i;
    }
    return i;
}

int chf_encode(chf_encoder *encoder, bitbuf *dst, uint16_t *sym, int n) {
    uint16_t r;
    for(int i = 0; i < n; ++i) {
        if(sym[i] > encoder->n) {
            return i;
        }

        uint16_t code = encoder->codes[sym[i]];
        r = encoder->lens[sym[i]];

        if(bitbuf_write_rev(dst, code, r) < 0) {
            return i;
        }
    }
    return n;
}

void chf_encoder_free(chf_encoder *encoder) {
    free(encoder->codes);
    free(encoder->lens);
}

int chf_encoder_init(chf_encoder *encoder, chf_code *chf) {
    encoder->n = chf->positions[chf->n - 1];
    encoder->codes = malloc(sizeof(uint16_t) * encoder->n);
    encoder->lens  = malloc(sizeof(uint8_t) * encoder->n);
    memset(encoder->lens, '\x00', sizeof(uint8_t) * encoder->n);
    if(encoder->codes == NULL || encoder->lens == NULL) {
        return -1;
    }
    uint8_t l = 1;
    uint16_t code = 0;
    for(int i = 0; i < encoder->n; ++i) {
        while(chf->positions[l - 1] <= i) {
            ++l;
            code <<= 1;
        }
        encoder->lens[chf->symbols[i]] = l;
        encoder->codes[chf->symbols[i]] = code;
        ++code;
    }
    return 0;
}
