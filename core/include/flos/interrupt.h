/**
 * @file    include/flos/interrupt.h
 * @brief   Interrupt
 * @version 1.0
 * @date    03.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__INTERRUPT_H__
#    define __flos__INTERRUPT_H__

#    include <flos/config.h>
#    include <flos/list.h>
#    include <flos/arch/registers.h>

#    define IRQ(x)                  (IRQ_OFFSET_MASTER + x)

#    define INTERRUPT_HANDLED       1
#    define INTERRUPT_NOT_HANDLED   0
#    define INTERRUPT_HANDLE_ERROR  -1

/**
 * Interrupt handler function
 * @return positive if handled, negative if error, 0 - not handled
 */
typedef int (ihandle_func_t) (struct iregs *);

struct interrupt_handle {
    ihandle_func_t *ihandle_func;

    struct list_head ihandle_list;
};

extern struct list_head interrupt_handler[INTERRUPT_COUNT];

void register_interrupt(int no, struct interrupt_handle *handle);
void unregister_interrupt(int no, struct interrupt_handle *handle);

void enable_interrupts();
void disable_interrupts();

#endif /* __flos__INTERRUPT_H__ */
