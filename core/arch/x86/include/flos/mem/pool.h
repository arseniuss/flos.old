/**
 * @file    arch/x86/include/flos/mem/pool.h
 * @brief   Memory pool
 * @version 1.0
 * @date    04.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__POOL_H__
#    define __flos_x86__POOL_H__

#    include <flos/mem/tlsf.h>

extern tlsf_t kernel_pool;

void init_pool();

void init_frames();

#endif /* __flos_x86__POOL_H__ */
