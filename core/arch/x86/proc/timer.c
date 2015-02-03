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

int init_timer() {
    kinfof("Initing timer ... ");

    pit_timer_phase(KERNEL_FREQ);

    kinfof(" OK\n");

    return 0;
}
