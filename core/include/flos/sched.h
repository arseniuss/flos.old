/**
 * @file    include/flos/sched.h
 * @brief   Scheduler
 * @version 1.0
 * @date    30.01.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */
#ifndef __flos__SCHED_H__
#    define __flos__SCHED_H__

#    define PRIORITY_COUNT          20
#    define PRIORITY_DEFAULT        10

/**
 * Schedule next process
 */
struct process *sched();

#endif /* __flos__SCHED_H__ */
