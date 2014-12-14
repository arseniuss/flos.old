/**
 * @file    arch/x86/init/kmain.c
 * @brief   x86 kernel main entry
 * @version 1.0
 * @date    28.11.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/kernel.h>

int kmain() {
    kprintf("kmain\n");
    return 0;
}
