/**
 * @file    mem/alloc.c
 * @brief   Memory allocation
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/defs.h>
#include <flos/kernel.h>
#include <flos/mem/pool.h>
#include <flos/mem/tlsf.h>
#include <flos/mem/tlsf.h>
#include <flos/vaargs.h>

void *kmalloc(size_t sz, int flags, ...) {
    va_list args;
    u32 align = 0;
    addr_t *phys;
    void *ret = NULL;

    va_start(args, flags);
    if(flags & KMALLOC_ALIGNED) {
        align = va_arg(args, u32);
    }
    if(flags & KMALLOC_PHYS) {
        phys = va_arg(args, addr_t *);
    }
    va_end(args);

    if(flags & KMALLOC_ALIGNED) {
        ret = tlsf_malign(kernel_pool, sz, align);
    } else {
        ret = tlsf_malloc(kernel_pool, sz);
    }

    if(ret == NULL)
        return NULL;

    if(flags & KMALLOC_PHYS) {
        //TODO: *phys = phys(ret);
    }

    return ret;
}

void kmfree(void *ptr) {
    tlsf_free(kernel_pool, ptr);
}
