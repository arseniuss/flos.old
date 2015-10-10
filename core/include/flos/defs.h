/**
 * @file    include/flos/defs.h
 * @brief   Definitions
 * @version 1.0
 * @date    08.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__DEFS_H__
#    define __flos__DEFS_H__

#    include <gcc/compiler.h>

/** Align structure */
#    define __align(x)              __attribute__((aligned(x)))
/** Force inlining */
#    define __always__inline        __attribute__((always_inline))
/** Assembly code */
#    define __asm                   __asm__ __volatile__
/** Do not inline */
#    define __dont_inline           __attribute__((noinline))
#    define __noinline              __dont_inline
/** Pack structures */
#    define __packed                __attribute__((packed))
/** Set variable as unused */
#    define __unused                __attribute__((unused))
/** Set variable as used */
#    define __used                  __attribute__((used))
/** Result must be checked */
#    define __must_check            __attribute__((warn_unused_result))
/** Specify section for data */
#    define __section(x)            __attribute__((section(x)))

#    define likely(x)               __builtin_expect(!!(x), 1)
#    define unlikely(x)             __builtin_expect(!!(x), 0)

#    define has_bit_flag(v, b)      ((v) & (0x1 << b))
#    define has_flag(v, f)          ((v) & (f))

#    ifdef __cplusplus
#        define __linkage           extern "C"
#    else
#        define __linkage
#    endif

#    ifndef typeof
#        define typeof(x)           __typeof__(x)
#    endif

/** Maximal character buffer length */
#    define MAX_CHAR_BUF_LEN        4096

#endif /* __flos__DEFS_H__ */
