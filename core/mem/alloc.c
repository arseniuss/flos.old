/**
 * @file    mem/alloc.c   
 * @brief   Memory allocation
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/kernel.h>
#include <flos/vaargs.h>

void init_pool() {
    
}

void *kmalloc(size_t sz, int flags, ...) {
    va_list args;
    u32 align = 0;
    void *phys = 0;

    va_start(args, flags);
    if(flags & KMALLOC_ALIGNED) {
        align = va_arg(args, u32);
    }
    if(flags & KMALLOC_PHYS) {
        phys = va_arg(args, void *);
    }
    va_end(args);
}
