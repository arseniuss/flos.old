/**
 * @file    arch/x86/proc/timer.c
 * @brief   x86 timer
 * @version 1.0
 * @date    03.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/arch/pit.h>
#include <flos/config.h>
#include <flos/kprintf.h>
#include <flos/interrupt.h>
#include <flos/init.h>

int timer_interrupt_handler(struct iregs *regs);

struct interrupt_handle timer_int = {
    &timer_interrupt_handler
};

int timer_interrupt_handler(struct iregs *regs) {
    return INTERRUPT_HANDLED;
}

int timer_init(void) {
    kinfof("Initing timer ... ");

    pit_timer_phase(KERNEL_FREQ);

    register_interrupt(IRQ(0), &timer_int);

    kinfof("OK\n");

    return 0;
}

KINIT(timer_init, "interrupts");
