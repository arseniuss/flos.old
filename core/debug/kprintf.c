/**
 * @file    debug/kprintf.c    
 * @brief   Kernel print (logging) function
 * @version 1.0
 * @date    13.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/defs.h>
#include <flos/kernel.h>
#include <flos/string.h>
#include <flos/vaargs.h>

#ifndef KLOG
#    error "Kernel logging function is not defined!"
#endif

extern int KLOG(int level, const char *buf);

int count = 0;

__linkage int early_kprintf(const char *str) {
    volatile char *video = (volatile char *) 0xB8000 + count * 2;

    while(*str) {
        *video++ = *str++;
        *video++ = 0x7;
        count++;
    }

    return 0;
}

__linkage int kprintf(const char *fmt, ...) {
    va_list args;
    int len;
    static char buf[MAX_CHAR_BUF_LEN];
    const char *text = buf;
    static int level;

    va_start(args, fmt);
    len = vsnprintf(buf, MAX_CHAR_BUF_LEN, fmt, args);
    va_end(args);

    int kern_level = kprintf_get_level(buf);
    if(kern_level) {
        text = kprintf_skip_level(buf);
        level = kern_level - '0';
    }

    KLOG(level, text);

    return len;
}
