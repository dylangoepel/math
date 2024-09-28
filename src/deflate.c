#include "huffman.h"
#include "gen/deflate.h"

void chf_set_deflate_fixed_lit(chf_code *chf) {
    chf->n = 9;
    chf->symbols = __fhf_lit_symbols;
    chf->positions = __fhf_lit_positions;
}

void chf_set_deflate_fixed_dist(chf_code *chf) {
    chf->n = 5;
    chf->symbols = __fhf_dist_symbols;
    chf->positions = __fhf_dist_positions;
}

/*
int chf_decode_dist(chf_decoder *ctx) {
    const int ret = chf_decode_next(ctx);
    if(ret < 0) {
        return ret;
    }
}
*/
