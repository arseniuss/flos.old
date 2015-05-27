/**
 * @file    arch/x86/mem/pool.c
 * @brief   x86 memory pool
 * @version 1.0
 * @date    04.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/config.h>
#include <flos/kprintf.h>
#include <flos/mem/pool.h>
#include <flos/mem/tlsf.h>
#include <flos/types.h>

tlsf_t kernel_pool = NULL;

void init_pool() {
    kprintf("Initing memory pool ... ");

    //TODO: change this to KERNEL_POOL_START
    kernel_pool = tlsf_create_in_pool((void *)KERNEL_POOL_START,
                                      KERNEL_INITIAL_POOL_SZ);

    kprintf("OK\n");
}
