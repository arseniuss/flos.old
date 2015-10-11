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
#include <flos/mem/area.h>
#include <flos/mem/phys.h>
#include <flos/config.h>
#include <flos/init.h>

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
               "Error: current process do not have %d. page table of address %p!",
               ptbl_idx, addr);

        ret = pt->entry[ptbl_idx] & PAGE_BITS;
        ret |= (addr_t) addr & PAGE_NOT_BITS;
    }

    return ret;
}

void paging_set_frame(addr_t virt_addr, addr_t phys_addr, int size_no) {
    int pdir_idx = PDIR_IDX(virt_addr);
    int ptbl_idx = PTBL_IDX(virt_addr);

    struct page_directory *pd = current->arch->page_directory;

    assert(pd);

    if(size_no == FRAME_SIZE1) {
        pd->entry[pdir_idx] = (phys_addr & PDIR_NOT_BITS)
            | PDE_PRESENT | PDE_RW | PDE_SIZE;
    } else if(size_no == FRAME_SIZE2) {
        struct page_table *pt = pd->table[pdir_idx];

        assert(pt);

        pt->entry[ptbl_idx] = (phys_addr & PAGE_NOT_BITS)
            | PTE_PRESENT | PTE_RW;
    } else {
        kerrorf("Error: wrong frame size type!");
        while(1);
    }
}

int page_fault_handler(struct iregs *regs) {
    addr_t fault_address;

  __asm("mov %%cr2, %0":"=r"(fault_address));

    assert(current, "Error: current process do not exist!");

    struct memarea_list *mal;

    list_for_each_entry(mal, &current->mmap_list, __mmap) {
        struct memarea *ma = mal->ma;

        assert(ma, "Error: NULL memory area!");

        if(fault_address >= ma->start && fault_address < ma->end) {
            kdebugf("Found fault address in memory area [%p .. %p]\n",
                    ma->start, ma->end);

            addr_t frame = frame_alloc(1);

            kdebugf("Allocated frame %p\n", frame);

            assert(frame && frame != MAX);

            paging_set_frame(fault_address, frame, FRAME_SIZE1);

            return INTERRUPT_HANDLED;
        }
    }

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

    return INTERRUPT_NOT_HANDLED;
}

int paging_init(void) {
    kprintf("Initing paging ... ");

    register_interrupt(PAGE_FAULT_INTERRUPT, &page_fault_handle);

    kprintf("OK\n");

    return 0;
}

KINIT(paging_init, "frames", "mem_area");
