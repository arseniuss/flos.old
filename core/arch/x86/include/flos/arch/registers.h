/**
 * @file    arch/x86/include/flos/arch/registers.h
 * @brief   Registers
 * @version 0.1
 * @date    23.04.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__REGISTERS_H__
#    define __flos_x86__REGISTERS_H__

/** Control Register flags @{ */

#    define CR0_PE  0x1             ///< Protection Enable
#    define CR0_MP  0x2             ///< Monitor coProcessor
#    define CR0_EM  0x4             ///< Emulation
#    define CR0_TS  0x8             ///< Task Switched
#    define CR0_ET  0x10            ///< Extension Type
#    define CR0_NE  0x20            ///< Numeric Error
#    define CR0_WP  0x10000         ///< Write Protect
#    define CR0_AM  0x40000         ///< Alignment Mask
#    define CR0_NW  0x20000000      ///< Not Writethrough
#    define CR0_CD  0x40000000      ///< Cache Disable
#    define CR0_PG  0x80000000      ///< Paging

#    define CR4_VME 0x01           ///< V86 Mode Extensions
#    define CR4_PVI 0x02           ///< Protected-Mode Virtual Interrupts
#    define CR4_TSD 0x04           ///< Time Stamp Disable
#    define CR4_DE  0x08           ///< Debugging Extensions
#    define CR4_PSE 0x10           ///< Page Size Extensions
#    define CR4_MCE 0x40           ///< Machine Check Enable
#    define CR4_PCE 0x100          ///< Performance counter enable
/** @} */

/** Eflags register @{ */
#    define EF_CF   0x1             ///< Carry Flag
#    define EF_PF   0x4             ///< Parity Flag
#    define EF_AF   0x10            ///< Auxiliary carry Flag
#    define EF_ZF   0x40            ///< Zero Flag
#    define EF_SF   0x80            ///< Sign Flag
#    define EF_TF   0x100           ///< Trap Flag
#    define EF_IF   0x200           ///< Interrupt Flag
#    define EF_DF   0x400           ///< Direction Flag
#    define EF_OF   0x800           ///< Overflow Flag
#    define EF_IOPL0 0x1000         ///< Low bit of I/O Privilege Level
#    define EF_IOPL1 0x2000         ///<High bit of I/O Privilege Level
#    define EF_NT   0x4000          ///< Nested Task
#    define EF_RF   0x10000         ///< Resume Flag
#    define EF_VM   0x20000         ///< Virtual 8086 mode
#    define EF_AC   0x40000         ///< Alignment Check
#    define EF_VIF  0x80000         ///< Virtual Interrupt Flag
#    define EF_VIP  0x100000        ///< Virtual Interrupt Pending
#    define EF_ID   0x200000        ///< ID flag
/** @} */

#    ifndef __PREPROCESSING__

#        include <flos/types.h>

/** Structure pushed by interrupt handler to stack */
struct iregs {
    u32 ss, gs, fs, es, ds; //push xs
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; //pusha
    u32 int_no, err_code; //push byte
    u32 eip, cs, eflags, esp3, ss3; //default
};
#    endif /* !__PREPROCESSING__ */
#endif /* !__flos_x86__REGISTERS_H__ */
