/**
 * @file    arch/x86/mem/area.c
 * @brief   x86 memory areas
 * @version 1.0
 * @date    04.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/arch/mem.h>
#include <flos/mem/area.h>
#include <flos/types.h>
#include <flos/process.h>
#include <flos/list.h>
#include <flos/kernel.h>
#include <flos/mem/phys.h>

addr_t kernel_virt_start = (addr_t) & __kernel_virt_start__;
addr_t kernel_virt_end = (addr_t) & __kernel_virt_end__;
addr_t kernel_text_start = (addr_t) & __kernel_text_start__;
addr_t kernel_text_end = (addr_t) & __kernel_text_end__;
addr_t kernel_rodata_start = (addr_t) & __kernel_rodata_start__;
addr_t kernel_rodata_end = (addr_t) & __kernel_rodata_end__;
addr_t kernel_data_start = (addr_t) & __kernel_data_start__;
addr_t kernel_data_end = (addr_t) & __kernel_data_end__;
addr_t kernel_bss_start = (addr_t) & __kernel_bss_start__;
addr_t kernel_bss_end = (addr_t) & __kernel_bss_end__;

addr_t kernel_symtbl_start = (addr_t) & __kernel_ksymtbl_start__;
addr_t kernel_symtbl_end = (addr_t) & __kernel_ksymtbl_end__;

struct memarea kernel_paging = {
    .start = (addr_t) & __kernel_paging_start__,
    .end = (addr_t) & __kernel_paging_end__,
    .flags = MA_RESERVED,
    .ops = NULL
};

struct memarea kernel_text = {
    .start = (addr_t) & __kernel_text_start__,
    .end = (addr_t) & __kernel_text_end__,
    .flags = MA_EXEC | MA_RESERVED,
    .ops = NULL
};

struct memarea kernel_rodata = {
    .start = (addr_t) & __kernel_rodata_start__,
    .end = (addr_t) & __kernel_rodata_end__,
    .flags = MA_READ | MA_RESERVED,
    .ops = NULL
};

struct memarea kernel_data = {
    .start = (addr_t) & __kernel_data_start__,
    .end = (addr_t) & __kernel_data_end__,
    .flags = MA_READ | MA_WRITE | MA_RESERVED,
    .ops = NULL
};

struct memarea kernel_bss = {
    .start = (addr_t) & __kernel_bss_start__,
    .end = (addr_t) & __kernel_bss_end__,
    .flags = MA_READ | MA_WRITE | MA_RESERVED,
    .ops = NULL
};

void mem_area_init(void) {
    struct memarea_list *node;

    INIT_LIST_HEAD(&current->mmap_list);

    frame_set(0x00000000, 0x00100000);

    node = early_kmalloc(sizeof(struct memarea_list), 0);
    node->ma = &kernel_paging;
    list_add_tail(&node->__mmap, &current->mmap_list);
    frame_set(node->ma->start - KERNEL_VIRTUAL_BASE,
              node->ma->end - node->ma->start);

    node = early_kmalloc(sizeof(struct memarea_list), 0);
    node->ma = &kernel_text;
    list_add_tail(&node->__mmap, &current->mmap_list);

    node = early_kmalloc(sizeof(struct memarea_list), 0);
    node->ma = &kernel_rodata;
    list_add_tail(&node->__mmap, &current->mmap_list);

    node = early_kmalloc(sizeof(struct memarea_list), 0);
    node->ma = &kernel_data;
    list_add_tail(&node->__mmap, &current->mmap_list);

    node = early_kmalloc(sizeof(struct memarea_list), 0);
    node->ma = &kernel_bss;
    list_add_tail(&node->__mmap, &current->mmap_list);
}
