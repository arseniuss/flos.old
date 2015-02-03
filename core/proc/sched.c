/**
 * @file    proc/sched.c
 * @brief   Scheduler
 * @version 1.0
 * @date    30.01.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/list.h>
#include <flos/sched.h>
#include <flos/kprintf.h>

#include "flos/process.h"

struct process *current;

struct list_head sched_list[PRIORITY_COUNT];
struct list_head current_list = LIST_HEAD_INIT(current_list);
int current_list = 0;

struct process *sched() {
    struct process *ret;
    int start_list = current_list;

    while(list_empty(current_list)) {
        current_list = (current_list + 1) % PRIORITY_COUNT;
        if(current_list == start_list)
            kwarningf("Scheduler list is empty!\n");
        list_splice_init(&sched_list[current_list], current_list);
    }

    ret = list_entry(current_list.next, struct process, sched_list);
    list_del_init(current_list.next);

    list_add(&current->sched_list,
             sched_list[(current->priority + current_list) % PRIORITY_COUNT]);

    return ret;
}

void init_sched() {
    for(int i = 0; i < PRIORITY_COUNT; ++i)
        sched_list[i] = LIST_HEAD_INIT(sched_list[i]);
}