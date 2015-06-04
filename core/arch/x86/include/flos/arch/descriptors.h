/**
 * @file    arch/x86/include/flos/arch/descriptors.h
 * @brief   Descriptos
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__DESCRIPTORS_H__
#    define __flos_x86__DESCRIPTORS_H__

/**
 * Application segment type bits @{
 */
#    define SEGA_ACCESSED               0x01    ///< Accessed
#    define SEGA_RW                     0x02    ///< Readable/Writeable
#    define SEGA_EC                     0x04    ///< Expand-down/Conforming
#    define SEGA_EXEC                   0x08    ///< Executable
/** @}*/

/**
 * System segment type bits @{
 */
#    define SEGT_16TSS_FREE             0x01    ///< Free 16bit TSS
#    define SEGT_LDT                    0x02    ///< Local Descriptor Table
#    define SEGT_16TSS_BUSY             0x03    ///< Busy 16bit TSS
#    define SEGT_16CALLGATE             0x04    ///< 16bit Call Gate
#    define SEGT_TASKGATE               0x05    ///< Task Gate
#    define SEGT_16INTGATE              0x06    ///< 16bit Interrupt Gate
#    define SEGT_16TRAPGATE             0x07    ///< 16bit Trap Gate
#    define SEGT_RESERVED1              0x08    ///< Reserved
#    define SEGT_32TSS_FREE             0x09    ///< Free 32bit TSS
#    define SEGT_RESERVED2              0x0A    ///< Reserved
#    define SEGT_32TSS_BUSY             0x0B    ///< Busy 32bit TSS
#    define SEGT_32CALLGATE             0x0C    ///< 32bit Call gate
#    define SEGT_RESERVED3              0x0D    ///< Reserved
#    define SEGT_32INTGATE              0x0E    ///< 32bit Interrupt Gate
#    define SEGT_32TRAPGATE             0x0F    ///< 32bit Trap Gate
/** @} */

/** Call Gate maxium supported param count */
#    define CALLGATE_MAX_PARAMS         0x1F
/** Currently used params for Call Gates */
#    define CALLGATE_PARAMS             0x00

#    ifndef __PREPROCESSING__

#        include <flos/defs.h>
#        include <flos/types.h>

/** Segment descriptor structure */
struct seg_desc {
    u16 low_limit;              ///< low bits of segment limit
    u16 low_base;               ///< low bits of segment base
    u8 mid_base;                ///< middle bits of segment base

    union {
        u8 flags;

        struct {
            u8 seg_type:4;      ///< segment type
            u8 desc_type:1;     ///< 0 - system, 1 - application
            u8 dpl:2;           ///< Descriptor Privilege Level
            u8 present:1;       ///< Present
        };
    };

    u8 high_limit:4;            ///< High bits of segment limit
    u8 __1:1;                   ///< unused
    u8 __2:1;                   ///< reserved (IA-32e)
    u8 db:1;                    ///< Default operation size (0 - 16bit, 1 - 32bit)
    u8 gran:1;                  ///< Granularity (0 - bytes, 1 - 4KB pages)
    u8 high_base;               ///< High bits of base
} __packed;

/** Segment creation macro */
#        define segment(type, base, lim, dpl)                                  \
            (struct seg_desc) {                                                \
                (lim) & 0xFFFF, (base) & 0xFFFF, ((base) >> 16) & 0xFF,        \
                type, 1, dpl, 1, (lim >> 16) & 0xF, 0, 0, 1, 1,                \
                ((base) >> 24) & 0xFF                                          \
            }

/** Gate descriptor structure */
struct gate_desc {
    u16 low_offset;             ///< low bits of offset in segment
    u16 ss;                     ///< segment selector
    u8 args;                    ///< argument count (for call gates)

    union {
        u8 flags;

        struct {
            u8 seg_type:4;      ///< segment type SEGT_*GATE
            u8 decr_type:1;     ///< descriptor type (must be 0 - system)
            u8 dpl:2;           ///< Descriptor Privilege Level
            u8 present:1;       ///< Present
        };
    };
    u16 high_offset;            ///< high bits of offset in segment
} __packed;

struct seg_ptr {
    u16 limit;
    u32 base;
} __packed;

extern struct seg_desc kernel_gdt[];
extern struct seg_ptr kernel_gdt_ptr;

static inline void idt_intr_gate(struct gate_desc *idt_entry,
                                 u16 seg, u32 offset, int dpl, int bits) {
    idt_entry->low_offset = offset & 0xFFFF;
    idt_entry->ss = seg;
    idt_entry->args = 0;
    idt_entry->flags = (bits ? SEGT_32INTGATE : SEGT_16INTGATE);
    idt_entry->flags |= (dpl & 3) << 5;
    idt_entry->flags |= 0x80;
    idt_entry->high_offset = (offset >> 16) & 0xFFFF;
}

static inline void idt_task_gate(struct gate_desc *idt_entry, u16 seg, int dpl) {
    idt_entry->low_offset = 0;
    idt_entry->ss = seg;
    idt_entry->args = 0;
    idt_entry->flags = SEGT_TASKGATE;
    idt_entry->flags |= (dpl & 3) << 5;
    idt_entry->flags |= 0x80;
    idt_entry->high_offset = 0;
}

static inline void idt_trap_gate(struct gate_desc *idt_entry,
                                 u16 seg, u32 offset, int dpl, int bits) {
    idt_entry->low_offset = offset & 0xFFFF;
    idt_entry->ss = seg;
    idt_entry->args = 0;
    idt_entry->flags = (bits ? SEGT_32TRAPGATE : SEGT_16TRAPGATE);
    idt_entry->flags |= (dpl & 3) << 5;
    idt_entry->flags |= 0x80;
    idt_entry->high_offset = (offset >> 16) & 0xFFFF;
}

static inline void flush_idt(struct seg_ptr *idt_ptr) {
  __asm("lidt %0"::"m"(*idt_ptr):"memory");
}

static inline void enable_interrupts() {
    __asm("sti");
}

static inline void disable_interrupts() {
    __asm("cli");
}

static inline void push_interrupts(volatile u32 * volatile buff) {
  __asm("pushf; pop %0; cli; ":"=g"(*buff));
}

static inline void pop_interrupts(volatile u32 * volatile buff) {
  __asm("push %0; popf;": :"g"(*buff));
}

extern struct gate_desc kernel_idt[];
extern struct seg_ptr kernel_idt_ptr;

__linkage int init_idt();

#    endif /* __PREPROCESSING__ */
#endif /* __flos_x86__DESCRIPTORS_H__ */
