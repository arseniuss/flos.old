/**
 * @file    include/flos/debug.h
 * @brief   Kernel debugging routines
 * @version 1.0
 * @date    04.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__DEBUG_H__
#    define __flos__DEBUG_H__

#    include <flos/defs.h>

#    define PRETEST(name)                                                      \
        int __section(".tests") name();                                        \
        int (*__##name##__ctor)() __section(".tests.ctors") __used             \
            = &name;

#    define POSTTEST(name)                                                     \
        int __section(".tests") name();                                        \
        int (*__##name##__dtor)() __section(".tests.dtors") __used             \
            = &name;

#    define TEST(name)          PRETEST(name)

#endif /* __flos__DEBUG_H__ */
