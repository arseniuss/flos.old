/**
 * @file    include/flos/kprintf.h   
 * @brief   Kernel logging function
 * @version 1.0
 * @date    13.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__KPRINTF_H__
#    define __flos__KPRINTF_H__

#    include <flos/defs.h>

#    define KERN_SOH                "\001"  /* ASCII Start Of Header */
#    define KERN_SOH_ASCII          '\001'

#    define KERN_EMERG              KERN_SOH "0"
#    define KERN_ALERT              KERN_SOH "1"
#    define KERN_CRIT               KERN_SOH "2"
#    define KERN_ERR                KERN_SOH "3"
#    define KERN_WARN               KERN_SOH "4"
#    define KERN_NOTICE             KERN_SOH "5"
#    define KERN_INFO               KERN_SOH "6"
#    define KERN_DEBUG              KERN_SOH "7"

static inline int kprintf_get_level(const char *buffer) {
    if(buffer[0] == KERN_SOH_ASCII && buffer[1]) {
        switch(buffer[1]) {
            case '0' ... '7':
                return buffer[1];
        }
    }

    return 0;
}

static inline const char *kprintf_skip_level(const char *buffer) {
    if(kprintf_get_level(buffer))
        return buffer + 2;

    return buffer;
}

__linkage int kprintf(const char *fmt, ...);

#    define kemergf(fmt, ...) \
                kprintf(KERN_EMERG fmt, ##__VA_ARGS__)
#    define kalertf(fmt, ...) \
                kprintf(KERN_ALERT fmt, ##__VA_ARGS__)
#    define kcritf(fmt, ...) \
                kprintf(KERN_CRIT fmt, ##__VA_ARGS__)
#    define kerrorf(fmt, ...) \
                kprintf(KERN_ERR fmt, ##__VA_ARGS__)
#    define kwarningf(fmt, ...) \
                kprintf(KERN_WARN fmt, ##__VA_ARGS__)
#    define knoticef(fmt, ...) \
                kprintf(KERN_NOTICE fmt, ##__VA_ARGS__)
#    define kinfof(fmt, ...) \
                kprintf(KERN_INFO fmt, ##__VA_ARGS__)
#    define kdebugf(fmt, ...) \
                kprintf(KERN_DEBUG fmt, ##__VA_ARGS__)

#endif /* __flos__KPRINTF_H__ */
