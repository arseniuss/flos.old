/**
 * @file    arch/x86/proc/interrupt.c
 * @brief
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/defs.h>

void enable_interrupts(void) {
    __asm("sti;");
}

void disable_interrupts(void) {
    __asm("cli;");
}
