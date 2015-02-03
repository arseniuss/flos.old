/**
 * @file    include/flos/process.h
 * @brief   Processes
 * @version 1.0
 * @date    30.01.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */
#ifndef __flos__PROCESS_H__
#    define __flos__PROCESS_H__

#    include <flos/arch/process.h>
#    include <flos/types.h>

struct process {
    pid_t pid;
    pid_t ppid;

    u8 priority;

    struct list_head sched_list;
};

#endif /* __flos__PROCESS_H__ */
