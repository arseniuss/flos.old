/**
 * @file    include/flos/kernel.h
 * @brief   Kernel main header
 * @version 1.0
 * @date    13.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__KERNEL_H__
#    define __flos__KERNEL_H__

#    include <flos/kprintf.h>
#    include <flos/types.h>

enum alloc_type {
    KMALLOC_ALIGNED,
    KMALLOC_PHYS,
    KMALLOC_ZERO
};

/**
 * Allocate memory for use in kernel only
 * @param sz size of memory chunk
 * @param flags
 * @param ... u32 align if flags & KMALLOC_ALIGNED;
 *              addr_t *phys if flags & KMALLOC_PHYS;
 * @return pointer to allocated chunk or NULL
 */
void *kmalloc(size_t sz, int flags, ...);
void *early_kmalloc(size_t sz, int flags, ...);
void kmfree(void *ptr);

#endif /* __flos__KERNEL_H__ */
