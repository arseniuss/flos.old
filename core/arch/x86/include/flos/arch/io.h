/**
 * @file    arch/x86/include/flos/arch/io.h     
 * @brief   Input/output functions
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__IO_H__
#define __flos_x86__IO_H__

#include <flos/defs.h>
#include <flos/types.h>

#define BUILDIO(bw, type)                                                  \
        static inline void out##bw(u16 port, type value) {                     \
            __asm("out" #bw " %" #bw "0, %w1" :: "a"(value), "Nd"(port));      \
        }                                                                      \
                                                                               \
        static inline void outs##bw(u16 port, const void *addr, int c) {       \
            __asm("cld repne outs" #bw :: "d"(port), "S"(addr), "c"(c) :       \
                "%esi", "%ecx");                                               \
        }                                                                      \
                                                                               \
        static inline type in##bw(u16 port) {                                  \
            type value;                                                        \
                                                                               \
            __asm("in" #bw " %w1, %" #bw "0" : "=a"(value) : "Nd"(port));      \
                                                                               \
            return value;                                                      \
        }                                                                      \
                                                                               \
        static inline void ins##bw(u16 port, void *addr, int c) {              \
            __asm("cld repne ins" #bw :: "d"(port), "D"(addr), "c"(c) :        \
                "%edi", "%ecx", "memory");                                     \
        }

BUILDIO (b, u8);
BUILDIO (w, u16);
BUILDIO (l, u32);

#endif /* __flos_x86__IO_H__ */
