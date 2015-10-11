/**
 * @file    include/flos/init.h
 * @brief   System initialization
 * @version 1.0
 * @date    11.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__INIT_H__
#    define __flos__INIT_H__

#    include <flos/defs.h>
#    include <flos/types.h>

int interrupts_init(void);

int kernel_init(void);

struct kinit;

struct kinit_deps {
    struct kinit *kinit;
    char *names[];
};

struct kinit {
    const char *name;
    int (*kinit) (void);
    bool inited;
    struct kinit_deps *deps;
};

#    define KINIT(func, ...) \
        static struct kinit_deps deps = { .names = { "", ## __VA_ARGS__, NULL } }; \
        static struct kinit UNIQUE(func) __section(".kinit") __used = \
            { .name = stringify(func), \
              .kinit = &func, \
              .deps = &deps };

#endif
