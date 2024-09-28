#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <stdint.h>
#include "bitbuf.h"

// canonical huffman code
typedef struct {
    uint16_t n, *positions, *symbols;
}chf_code;

// keeps track of current code prefix and length during decoding
typedef struct {
    chf_code *chf;
    uint32_t *start; // pre-computed start codes
                     // chf->positions[start[i]]
    uint8_t l; // prefix length
    uint32_t c; // prefix 
}chf_decoder;

// bit-level dictionary, in-memory repr optimized for encoding
// codes[i] contains bitcode of length lens[i] representing i
typedef struct {
    uint32_t n;
    uint16_t *codes;
    uint8_t *lens;
}chf_encoder;

void chf_decoder_init(chf_decoder*);
int chf_decoder_load(chf_decoder*, chf_code*);

int chf_decode_bit(chf_decoder*, uint8_t);
int chf_decode_symbol(chf_decoder*, bitbuf*);
int chf_decode(chf_decoder*, bitbuf*, uint16_t[], uint16_t);

int chf_encoder_init(chf_encoder*, chf_code*);
void chf_encoder_free(chf_encoder*);
int chf_encode(chf_encoder*, bitbuf*, uint16_t*, int);

#endif
