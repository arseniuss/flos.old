/**
 * @file    arch/x86/init/idt.c
 * @brief   Interrupt Descriptor Table
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/arch/descriptors.h>
#include <flos/arch/pic.h>
#include <flos/arch/registers.h>
#include <flos/config.h>
#include <flos/interrupt.h>
#include <flos/kernel.h>
#include <flos/list.h>
#include <flos/string.h>
#include <flos/types.h>
#include <flos/init.h>

struct seg_ptr idt_ptr;
struct gate_desc idt_table[INTERRUPT_COUNT];
extern u32 idt_vectors[];

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

int idt_init(void) {
    kinfof("Initing interrupts ... ");

    pic_remap(IRQ_OFFSET_MASTER, IRQ_OFFSET_SLAVE);

    idt_ptr.limit = sizeof(struct gate_desc) * INTERRUPT_COUNT - 1;
    idt_ptr.base = (u32) & idt_table;

    memset(&idt_table, 0, sizeof(struct gate_desc) * INTERRUPT_COUNT);

    for(int i = 0; i <= IRQ_ISR_VECTORS; i++) {
        idt_trap_gate(&idt_table[i], KERNEL_CS, idt_vectors[i], 0, 1);
    }

    flush_idt(&idt_ptr);
    __asm("sti;");

    kinfof(" OK\n");

    return 0;
}

struct iregs *irq_handler(struct iregs *regs) {
    int irq = regs->int_no - IRQ_OFFSET_MASTER;
    struct interrupt_handle *pos;
    int handled = INTERRUPT_NOT_HANDLED;

    list_for_each_entry(pos, &interrupt_handler[regs->int_no], ihandle_list) {
        handled = pos->ihandle_func(regs);

        if(handled <= INTERRUPT_HANDLE_ERROR)
            kerrorf("Error while handing IRQ%d\n", irq);
        if(handled >= INTERRUPT_HANDLED)
            break;
    }

    if(handled <= INTERRUPT_NOT_HANDLED)
        kdebugf_once("IRQ%d is not handled\n", irq);

    pic_eoi(irq);

    return regs;
}

struct iregs *isr_handler(struct iregs *regs) {
    struct interrupt_handle *pos;
    int handled = INTERRUPT_NOT_HANDLED;

    list_for_each_entry(pos, &interrupt_handler[regs->int_no], ihandle_list) {
        handled = pos->ihandle_func(regs);

        if(handled <= INTERRUPT_HANDLE_ERROR)
            kerrorf("Error while handing interrupt %d\n", regs->int_no);
        if(handled >= INTERRUPT_HANDLED)
            break;
    }

    if(handled <= INTERRUPT_NOT_HANDLED) {
        kcritf("Unresolved exception no. %d: %s\n", regs->int_no,
               exception_messages[regs->int_no]);
        dump_regs(regs);

        while(1);
    }

    return regs;
}

KINIT(idt_init, "interrupts");
