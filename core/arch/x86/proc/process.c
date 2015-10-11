/**
 * @file    arch/x86/proc/process.c
 * @brief
 * @version 1.0
 * @date    11.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/kernel.h>
#include <flos/proc/arch.h>
#include <flos/process.h>
#include <flos/mem/paging.h>
#include <flos/sched.h>

extern struct page_directory kernel_page_directory;

struct process_arch root_process_arch = {
    .page_directory = &kernel_page_directory
};

struct process root_process = {
    .arch = &root_process_arch
};

struct process *current = &root_process;

struct process_arch *process_arch_alloc(void) {
    struct process_arch *ret = (struct process_arch *)
        kmalloc(sizeof(struct process_arch), KMALLOC_ZERO);

    ret->page_directory = (struct page_directory *)
        kmalloc(sizeof(struct page_directory), KMALLOC_ZERO);

    return ret;
}

int process_init(void) {
    root_process.pid = 0;
    root_process.ppid = 0;
    root_process.priority = PRIORITY_DEFAULT;

    INIT_LIST_HEAD(&root_process.sched_list);
    INIT_LIST_HEAD(&root_process.mmap_list);

    kinfof("Processes inited!\n");

    return 0;
}
