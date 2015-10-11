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
#include <flos/types.h>

void enable_interrupts(void) {
    __asm("sti;");
}

void disable_interrupts(void) {
    __asm("cli;");
}

void push_interrupts(volatile __unsigned * volatile buff) {
  __asm("pushf; pop %0; cli; ":"=g"(*buff));
}

void pop_interrupts(volatile __unsigned * volatile buff) {
  __asm("push %0; popf;": :"g"(*buff));
}
