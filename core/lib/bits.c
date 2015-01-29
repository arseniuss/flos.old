/**
 * @file    lib/bits.c
 * @brief   Bits functions
 * @version 0.1
 * @date    09.06.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

static int fls_generic(int word) {
    int bit = 32;

    if(!word)
        bit -= 1;
    if(!(word & 0xffff0000)) {
        word <<= 16;
        bit -= 16;
    }
    if(!(word & 0xff000000)) {
        word <<= 8;
        bit -= 8;
    }
    if(!(word & 0xf0000000)) {
        word <<= 4;
        bit -= 4;
    }
    if(!(word & 0xc0000000)) {
        word <<= 2;
        bit -= 2;
    }
    if(!(word & 0x80000000)) {
        word <<= 1;
        bit -= 1;
    }

    return bit;
}

int fls(int word) {
    return fls_generic(word) - 1;
}

int ffs(int word) {
    return fls_generic(word & (~word + 1)) - 1;
}
