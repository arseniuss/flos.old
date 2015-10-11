/**
 * @file    arch/x86/syscall.h
 * @brief   System calls
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

/**
 * This system uses callgates
 */

#ifndef __SYSCALL__
#    define __SYSCALL__

#    define DECL_SYSCALL0(fn)                   int syscall_##fn();
#    define DECL_SYSCALL1(fn,p1)                int syscall_##fn(p1);
#    define DECL_SYSCALL2(fn,p1,p2)             int syscall_##fn(p1,p2);
#    define DECL_SYSCALL3(fn,p1,p2,p3)          int syscall_##fn(p1,p2,p3);
#    define DECL_SYSCALL4(fn,p1,p2,p3,p4)       int syscall_##fn(p1,p2,p3,p4);
#    define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5)    int syscall_##fn(p1,p2,p3,p4,p5);

#    define DEFN_SYSCALL0(fn, num)                                                 \
            int syscall_##fn() {                                               \
                int ret;                                                       \
                __asm__ __volatile__(".byte 0x9a;"                             \
                    ".long 0x0;"                                               \
                    ".word 0x30;":"=a"(ret));                                  \
                return ret;                                                    \
             };

#    define DEFN_SYSCALL1(fn, num, P1)                                             \
            int syscall_##fn(P1 p1) {                                          \
                int ret;                                                       \
                __asm__ __volatile__(".byte 0x9a;"                             \
                    ".long 0x0;"                                               \
                    ".word 0x30;":"=a"(ret):"0"(p1));                          \
                return ret;                                                    \
             };

#    define DEFN_SYSCALL2(fn, num, P1, P2)                                         \
            int syscall_##fn(P1 p1, P2 p2) {                                   \
                int ret;                                                       \
                __asm__ __volatile__("push %0;"::"r"(p2));                     \
                __asm__ __volatile__("push %0;"::"r"(p1));                     \
                __asm__ __volatile__("push %0;"::"r"(num));                    \
                __asm__ __volatile__(".byte 0x9a;"                             \
                    ".long 0x0;"                                               \
                    ".word 0x30;":"=a"(ret):"0"(p1),"b"(p2));                  \
                return ret;                                                    \
             };


#    define DEFN_SYSCALL3(fn, num, P1, P2, P3)                                     \
            int syscall_##fn(P1 p1, P2 p2, P3 p3) {                            \
                int ret;                                                       \
                __asm__ __volatile__("push %0;"::"r"(p3));                     \
                __asm__ __volatile__("push %0;"::"r"(p2));                     \
                __asm__ __volatile__("push %0;"::"r"(p1));                     \
                __asm__ __volatile__("push %0;"::"r"(num));                    \
                __asm__ __volatile__(".byte 0x9a;"                             \
                    ".long 0x0;"                                               \
                    ".word 0x30;":"=a"(ret):"0"(p1),"b"(p2),"c"(p3));          \
                return ret;                                                    \
             };


#    define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4)                                 \
            int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4) {                     \
                int ret;                                                       \
                __asm__ __volatile__("push %0;"::"r"(p4));                     \
                __asm__ __volatile__("push %0;"::"r"(p3));                     \
                __asm__ __volatile__("push %0;"::"r"(p2));                     \
                __asm__ __volatile__("push %0;"::"r"(p1));                     \
                __asm__ __volatile__("push %0;"::"r"(num));                    \
                __asm__ __volatile__(".byte 0x9a;"                             \
                    ".long 0x0;"                                               \
                    ".word 0x30;":"=a"(ret):                                   \
                    "0"(p1), "b"(p2), "c"(p3), "d"(p4));                       \
                return ret;                                                    \
             };


#    define DEFN_SYSCALL5(fn, num, P1, P2, P3, P4, P5)                             \
            int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {              \
                int ret;                                                       \
                __asm__ __volatile__(".byte 0x9a;"                             \
                    ".long 0x0;"                                               \
                    ".word 0x30;":"=a"(ret):                                   \
                    "0"(p1), "b"(p2), "c"(p3), "d"(p4), "S"(p5));              \
                return ret;                                                    \
             };

#endif
