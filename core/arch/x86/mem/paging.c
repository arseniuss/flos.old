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
#include <flos/process.h>
#include <flos/assert.h>

#define PAGE_SHIFT      12
#define PAGE_SIZE       (1 << PAGE_SHIFT)
#define PAGE_BITS       0x00000FFF
#define PAGE_NOT_BITS   0xFFFFF000

#define PDIR_SHIFT      22
#define PDIR_SIZE       (1 << PDIR_SHIFT)
#define PDIR_BITS       0x003FFFFF
#define PDIR_NOT_BITS   0xFFC00000

#define PDIR_IDX(x)     (((addr_t)x) >> PDIR_SHIFT)
#define PTBL_IDX(x)     ((((addr_t)x) >> PAGE_SHIFT) & 0x3FF)

int page_fault_handler(struct iregs *regs);

struct interrupt_handle page_fault_handle = {
    &page_fault_handler
};

addr_t phys(void *addr) {
    int pdir_idx = PDIR_IDX(addr);
    int ptbl_idx = PTBL_IDX(addr);
    struct page_directory *pd = current->arch->page_directory;
    addr_t ret = 0;

    assert(pd != NULL, "Error: current process do not have page directory!");

    if(pd->entry[pdir_idx] & PDE_SIZE) {
        ret = pd->entry[pdir_idx] & PDIR_NOT_BITS;
        ret |= (addr_t) addr & PDIR_BITS;
    } else {
        struct page_table *pt = pd->table[ptbl_idx];

        assert(pt != NULL,
               "Error: current process do not have %d page table!", ptbl_idx);

        ret = pt->entry[ptbl_idx] & PAGE_BITS;
        ret |= (addr_t) addr & PAGE_NOT_BITS;
    }

    return ret;
}

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
