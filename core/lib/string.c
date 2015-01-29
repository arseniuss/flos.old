/**
 * @file    lib/string.c    
 * @brief   String functions
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/string.h>
#include <flos/types.h>

size_t strnlen(const char *s, size_t count) {
    const char *sc;

    for(sc = s; count-- && *sc != '\0'; ++sc)
        /*
         * nothing 
         */ ;
    return sc - s;
}

size_t strlen(const char *s) {
    const char *sc;

    for(sc = s; *sc != '\0'; ++sc)
        /*
         * nothing 
         */ ;

    return sc - s;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    unsigned char *p1 = (unsigned char *)s1;
    unsigned char *p2 = (unsigned char *)s2;

    while(n--) {
        if(*p1 != *p2) {
            return *p1 - *p2;
        }
        ++p1;
        ++p2;
    }

    return 0;
}

void *memcpy(void *dst, const void *src, size_t n) {
    char *dp = dst;
    const char *sp = src;

    for(; n--; dp++, sp++)
        *dp = *sp;

    return dst;
}

void *memset(void *dst, int c, unsigned int n) {
    char *d = (char *)dst;

    for(; n > 0; --n)
        *d++ = (char)c;

    return dst;
}
