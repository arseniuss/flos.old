/**
 * @file    arch/x86/proc/syscall.c
 * @brief   System calls
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/config.h>
#include <flos/arch/descriptors.h>
#include <flos/kprintf.h>

struct gdt_callgate_entry_s {
    u16 offset_low;
    u16 seg;

    union {

        struct {
            u8 params:5;
            u8 __1:3;
        };
    };

    union {

        struct {
            u8 type:4;          //0b1100;
            u8 __2:1;
            u8 dpl:2;
            u8 present:1;
        };
        u8 flags;
    };
    u16 offset_high;
};

extern void callgate_entry();

int callgate_handler() {
    kdebugf("Callgate\n");

    return 0;
}

void callgate_init(void) {
    struct gdt_callgate_entry_s *entry =
        (struct gdt_callgate_entry_s *)&kernel_gdt[GDT_SEGSYSCALL];

    entry->offset_low = ((u32) callgate_entry) & 0xFFFF;
    entry->offset_high = (((u32) callgate_entry) >> 16) & 0xFFFF;
    entry->seg = KERNEL_CS & 0xFFFF;
    entry->params = CALLGATE_PARAMS & 0x1F;
    entry->flags = SEGT_32CALLGATE | 0x60 | 0x80;

    flush_gdt(&kernel_gdt_ptr);

    kinfof("Callgates inited!\n");
}
