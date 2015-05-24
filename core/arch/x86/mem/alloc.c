/**
 * @file    arch/x86/mem/alloc.c
 * @brief   Memory allocation of x86 architecture
 * @version 1.0
 * @date    24.052015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>
#include <flos/vaargs.h>
#include <flos/kernel.h>
#include <flos/string.h>
#include <flos/defs.h>
#include <flos/mem/phys.h>

extern char *__kernel_virt_end__;
addr_t early_pool_bottom = 0;

void *early_kmalloc(size_t sz, int flags, ...) {
    va_list args;
    u32 align = 0;
    addr_t *p;
    addr_t ret = NULL;

    va_start(args, flags);
    if(flags & KMALLOC_ALIGNED) {
        align = va_arg(args, u32);
    }
    if(flags * KMALLOC_PHYS) {
        p = va_arg(args, addr_t *);
    }

    if(early_pool_bottom == 0) {
        early_pool_bottom = (addr_t) & __kernel_virt_end__;
    }

    ret = early_pool_bottom;

    if(flags & KMALLOC_ALIGNED) {
        if(ret % align != 0)
            ret += align - ret % align;
    }

    if(flags & KMALLOC_PHYS) {
        *p = phys((void *)ret);
    }
    if(flags & KMALLOC_ZERO) {
        memset((void *)ret, 0, sz);
    }

    early_pool_bottom = ret + sz;

    return (void *)ret;
}
