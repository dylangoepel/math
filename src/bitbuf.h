#ifndef BITBUF_H_
#define BITBUF_H_

#include <stdint.h>

typedef struct {
    uint8_t k, *b;
    uint16_t len;
}bitbuf;

static inline bitbuf bitbuf_open(uint8_t *b, uint16_t len) {
    bitbuf buf;
    buf.k = 8;
    buf.b = b - 1;
    buf.len = len;
    return buf;
}

static inline int bitbuf_read(bitbuf* buf) {
    if(buf->k == 8) {
        if(buf->len == 0) {
            return -1;
        }
        buf->k = 0;
        ++buf->b;
        --buf->len;
    }
    uint8_t bit = (buf->b[0] >> buf->k) & 1;
    ++buf->k;
    return bit;
}

static inline int bitbuf_write(bitbuf* dst, uint8_t bit) {
    if(dst->k == 8) {
        if(dst->len == 0) {
            return -1;
        }
        ++dst->b;
        --dst->len;
        dst->k = 0;
        dst->b[0] = 0;
    }
    if(bit) {
        dst->b[0] |= 1 << dst->k;
    }
    ++dst->k;
    return 0;
}

static inline int bitbuf_write_rev(bitbuf* dst, uint32_t c, int n) {
    int ret;
    for(int i = 0; i < n; ++i) {
        ret = bitbuf_write(dst, (c >> (n - i - 1)) & 1);
        if(ret < 0) {
            return i;
        }
    }
    return n;
}

static inline int bitbuf_write_n(bitbuf* dst, uint32_t c, int n) {
    int ret;
    for(int i = 0; i < n; ++i) {
        ret = bitbuf_write(dst, (c >> i) & 1);
        if(ret < 0) {
            return i;
        }
    }
    return n;
}

#endif
