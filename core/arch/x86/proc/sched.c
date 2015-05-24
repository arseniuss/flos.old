/**
 * @file    arch/x86/proc/sched.c
 * @brief   x86 scheduer variables
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/process.h>
#include <flos/mem/paging.h>

extern struct page_directory kernel_page_directory;

struct process_arch root_process_arch = {
    .page_directory = &kernel_page_directory
};

struct process root_process = {
    .arch = &root_process_arch
};

struct process *current = &root_process;
