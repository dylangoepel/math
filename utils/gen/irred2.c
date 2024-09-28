#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define _SRC_NAME "utils/gen/irred2.c"
#define _COMMENT "minimal irreducible polynomials in Z_2[x] for degree n <= 20."
#define _SYM "_irr2_min"
#define _MAXDEG 20

/* source formatting */
#define _generated_source_note "generated source. see " _SRC_NAME
#define _comment(s) "/* " s " */\n"
#define _include(s) "#include<" s ">\n"
#define src_pre _comment(_COMMENT) \
            _comment(_generated_source_note) \
            _include("stdint.h")

#define write_str(s) fwrite(s, 1, strlen(s), stdout)

const static char *def_u32s_pre = "uint32_t %s[]={";
const static char *def_u32s_post = "};\n";
#define write_u32(p, withDelim) ((withDelim) ? printf("0x%x,", p) : printf("0x%x", p))
#define write_u32s(name, ps, n) { \
    printf(def_u32s_pre, name); \
    for(int i=0; i<n; ++i) write_u32(ps[i], i < n - 1); \
    write_str(def_u32s_post); \
}

/* find minimal irreducible polynomials in Z_2[x] of degrees n = 1, ..., poly_max. */
void gen_irred_until(uint32_t *out, const uint8_t poly_max);

int main() {
    uint32_t irred[_MAXDEG]; 

    gen_irred_until(irred, _MAXDEG);

    write_str(src_pre);
    write_u32s(_SYM, irred, _MAXDEG);

    return 0;
}

/* prime sieve over Z_2[x] range 1,...,x^(poly_max + 1) */
void gen_irred_until(uint32_t *out, const uint8_t poly_max) {
    uint8_t isReducible[2 << poly_max], i = 1;
    uint32_t x, pr, ps, qs;

    memset(isReducible, 0, 2 << poly_max);
    for(uint32_t p = 2; 1; ++p) {
        if(isReducible[p]) { /* move to next p not marked as reducible */
            continue;
        }

        if(p >= 1 << i) { /* output if is first of degree i */
            out[i - 1] = p;
            ++i;
            if(i > poly_max) return;
        }

        /* mark multiples as reducible */
        for(x = 2; 1; ++x) {
            pr = 0;
            ps = x;
            qs = p;

            while(ps != 0) { /* compute pr = x * p */
                if(ps & 1) {
                    pr ^= qs;
                }
                qs <<= 1;
                ps >>= 1;
            }

            if(pr >= (2 << poly_max)) {
                break;
            }
            isReducible[pr] = 1; 
        }
    }
}
