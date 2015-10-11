/**
 * @file    proc/process.c
 * @brief   Process
 * @version 1.0
 * @date    11.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/mem/area.h>
#include <flos/process.h>
#include <flos/kernel.h>
#include <flos/sched.h>
#include <flos/list.h>
#include <flos/arch/process.h>
#include <flos/kprintf.h>

static pid_t last_pid = 1;
pid_t process_new_pid(void) {
    return last_pid++;
}

struct process_arch *process_arch_alloc(void);

struct process *process_alloc(void) {
    struct process *ret = (struct process *)kmalloc(sizeof(struct process),
                                                    KMALLOC_ZERO);

    ret->pid = process_new_pid();
    ret->ppid = 0;
    ret->priority = PRIORITY_DEFAULT;
    INIT_LIST_HEAD(&ret->sched_list);
    INIT_LIST_HEAD(&ret->mmap_list);

    ret->arch = process_arch_alloc();

    return ret;
}

extern struct process root_process;
