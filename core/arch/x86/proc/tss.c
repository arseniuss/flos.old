/**
 * @file    arch/x86/proc/tss.c
 * @brief   x86 task state structure
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/config.h>
#include <flos/arch/descriptors.h>
#include <flos/init.h>
#include <flos/arch/tss.h>

struct tss kernel_tss;

int tss_init(void) {
    return 0;
}

KINIT(tss_init);
