/**
 * @file    arch/x86/mem/paging.c
 * @brief   x86 paging
 * @version 1.0
 * @date    04.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/arch/registers.h>
#include <flos/mem/paging.h>
#include <flos/interrupt.h>
#include <flos/kprintf.h>

int page_fault_handler(struct iregs *regs);

struct interrupt_handle page_fault_handle = {
    &page_fault_handler
};

int page_fault_handler(struct iregs *regs) {
    addr_t fault_address;

  __asm("mov %%cr2, %0":"=r"(fault_address));

    int present = regs->err_code & 0x1;
    int rw = regs->err_code & 0x2;
    int us = regs->err_code & 0x4;
    int reserved = regs->err_code & 0x08;
    int id = regs->err_code & 0x10;

    kcritf("Page fault @ %p ", fault_address);
    if(!present)
        kcritf("!present ");
    if(rw)
        kcritf("read-only ");
    if(us)
        kcritf("used ");
    if(reserved)
        kcritf("reserved ");
    if(id)
        kcritf("ID ");
    kcritf("\n");

    return 0;
}

void init_memory() {
    kprintf("Initing paging ... ");

    register_interrupt(PAGE_FAULT_INTERRUPT, &page_fault_handle);

    kprintf("OK\n");
}
