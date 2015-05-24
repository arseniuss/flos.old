/**
 * @file    proc/interrupts.c
 * @brief   Interrupt handler
 * @version 1.0
 * @date    03.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/list.h>
#include <flos/config.h>
#include <flos/interrupt.h>
#include <flos/assert.h>

struct list_head interrupt_handler[INTERRUPT_COUNT];

int init_interrupts() {
    for(int i = 0; i < INTERRUPT_COUNT; ++i) {
        INIT_LIST_HEAD(&interrupt_handler[i]);
    }

    return 0;
}

void register_interrupt(int no, struct interrupt_handle *handle) {
    assert(handle);
    assert(handle->ihandle_func);

    list_add(&handle->ihandle_list, &interrupt_handler[no]);
}

void unregister_interrupt(int no, struct interrupt_handle *handle) {
    list_del_init(&handle->ihandle_list);
}
