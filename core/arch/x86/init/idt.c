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
#include <flos/kernel.h>
#include <flos/string.h>
#include <flos/types.h>

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

int init_idt() {
    kinfof("Initing interrupts ... ");

    pic_remap(0x20, 0x28);

    idt_ptr.limit = sizeof(struct gate_desc) * INTERRUPT_COUNT - 1;
    idt_ptr.base = (u32) & idt_table;

    memset(&idt_table, 0, sizeof(struct gate_desc) * INTERRUPT_COUNT);

    for(int i = 0; i <= 47; i++) {
        idt_trap_gate(&idt_table[i], KERNEL_CS, idt_vectors[i], 0, 1);
    }

    flush_idt(&idt_ptr);
    __asm("sti");

    kinfof(" OK\n");

    return 0;
}

struct iregs *irq_handler(struct iregs *regs) {
    kprintf("IRQ %d\n", regs->int_no);

    return regs;
}

struct iregs *isr_handler(struct iregs *regs) {
    u32 fault_addr;

    kprintf("ISR: %p %s ", regs->eip, exception_messages[regs->int_no]);

    switch (regs->int_no) {
        case 14:
          __asm("mov %%cr2, %0":"=r"(fault_addr));

            kprintf("@ %p\n", fault_addr);
            break;
    }

    for(;;);

    return regs;
}
