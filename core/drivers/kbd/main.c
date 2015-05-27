/**
 * @file    drivers/kbd/main.c
 * @brief   Keyboard driver
 * @version 0.1
 * @date    24.05.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>
#include <flos/interrupt.h>
#include <flos/arch/io.h>

#define DATA_PORT           0x60
#define STAT_PORT           0x64
#define CMD_PORT            0x64

#define STAT_OUPUT_FULL     0x01
#define STAT_INPUT_FULL     0x02
#define STAT_SYS_FLAG       0x04
#define STAT_IS_CMD         0x08
#define STAT_UNKNOWN1       0x10
#define STAT_UNKNOWN2       0x20
#define STAT_TIMEOUT        0x40
#define STAT_PARITY         0x80

int kbd_init();
int kbd_interrupt(struct iregs *regs);
void kbd_exit();

struct module kdb_module = {
    .name = "kdb",
    .init = &kbd_init,
    .exit = &kbd_exit
};

struct interrupt_handle kbd_interrupt_handle = {
    .ihandle_func = &kbd_interrupt
};


int kbd_init() {
    kinfof("Initing keyboard driver ... ");

    register_interrupt(IRQ(1), &kbd_interrupt_handle);

    kinfof("OK\n");

    return 0;
}

int kbd_interrupt(struct iregs *regs) {
    inb(DATA_PORT);

    return INTERRUPT_HANDLED;
}

void kbd_exit() {
    unregister_interrupt(IRQ(1), &kbd_interrupt_handle);
}

INTERNAL_MODULE(kdb_module);
