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

#include "log.h"

log_puts_t *kernel_log_handle;

__linkage int kprintf(const char *fmt, ...) {
    va_list args;
    int len;
    static char buf[MAX_CHAR_BUF_LEN];
    
    if(!kernel_log_handle)
        return 0;
    
    va_start(args, fmt);
    len = vsnprintf(buf, MAX_CHAR_BUF_LEN, fmt, args);
    va_end(args);
    
    kernel_log_handle(buf);
    
    return len;
}
