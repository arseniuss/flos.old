/**
 * @file    include/gcc/compiler.h
 * @brief   GCC definitions and checks
 * @version 0.1
 * @date    24.04.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__GCC_COMPILER_H__
#    define __flos__GCC_COMPILER_H__

#    define GCC_VERSION                                                        \
        (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#    if GCC_VERSION < 40300
#        error "TODO: __COUNTER__ alternative"
#    endif

#    define stringify(x)                #x
#    define __CONCAT(x, y)              x##y
#    define CONCAT(x, y)                __CONCAT(x, y)

#    define UNIQUE(name)                CONCAT(name, __COUNTER__)

#    define offsetof(type, member)      __builtin_offsetof (type, member)

#endif /* !__flos__GCC_COMPILER_H__ */
