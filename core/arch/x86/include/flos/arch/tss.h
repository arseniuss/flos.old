/**
 * @file    arch/x86/include/flos/arch/tss.h
 * @brief   Task state structure
 * @version 0.1
 * @date    10.10.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__TSS_H__
#    define __flos_x86__TSS_H__

#    include <flos/defs.h>
#    include <flos/types.h>

struct tss {
    u32 prev_tss;
    u32 esp0;
    u32 ss0;
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldt;
    u16 trap;
    u16 iomap_base;
} __packed;

void tss_init(void);

#endif
