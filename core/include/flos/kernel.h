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
};

__linkage void *kmalloc(size_t sz, int flags, ...);

#endif /* __flos__KERNEL_H__ */
