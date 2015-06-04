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
#include <flos/assert.h>
#include <flos/mem/pool.h>
#include <flos/arch/mem.h>

addr_t early_pool_start = NULL;
addr_t early_pool_end = NULL;

void *early_kmalloc(size_t sz, int flags, ...) {
    va_list args;
    u32 align = 0;
    addr_t *p;
    addr_t ret = NULL;

    assert(kernel_pool == NULL,
           "Error: cannot use early_kmalloc, memory pool exists!");

    va_start(args, flags);
    if(flags & KMALLOC_ALIGNED) {
        align = va_arg(args, u32);
    }
    if(flags * KMALLOC_PHYS) {
        p = va_arg(args, addr_t *);
    }

    if(early_pool_end == NULL) {
        early_pool_start = kernel_virt_end;
        early_pool_end = kernel_virt_end;
    }

    ret = early_pool_end;

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

    early_pool_end = ret + sz;

    return (void *)ret;
}
