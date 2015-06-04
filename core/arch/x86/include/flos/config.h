/**
 * @file    arch/x86/config.h
 * @brief   x86 configuration header
 * @version 1.0
 * @date    29.11.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__config_h__
#    define __flos_x86__config_h__

/*
 * Virtual Memory Map:
 *
 *      4 GB            +-------------------------------------------+ 0xFFFFFFFF
 *                      |                                           | \
 *                      |       (unallocated protection page)       |  | 4 KB
 *                      |                                           | /
 *                      +------------------------------------------ + 0xFFFFE000
 *                      |                                           | \
 *                      |                                           |  |
 *                      |                                           |  | ~240 MB
 *                      |               ^ grows up ^                |  |
 *                      |            (kernel memory pool)           | /
 * KERNEL_STACK_START   +-------------------------------------------+ 0xF0800000
 *                      |              (kernel stack)               | \
 *                      |              V grows down V               |  |
 *                      |  +---------------+                        |  |
 *                      |  | iregs.ss3 = . |                        |  |
 *                      |  | iregs.esp3 = .|                        |  |
 *                      |  |               |                        |  | 4092 KB
 *                      |  |     ...       |                        |  |
 *                      |  +---------------+                        |  |
 *                      |                                           |  |
 *                      |                                           |  |
 *                      |                                           | /
 *                      +-------------------------------------------+
 *                      |                                           | \
 *                      |       (unallocated protection page)       |  | 4 KB
 *                      |                                           | /
 * KERNEL_STACK_END     +-------------------------------------------+
 *                      |                                           |
 *                      |                                           |
 *                      |               ^ grows up ^                |
 *                      |             static kernel data            |
 *                      +-------------------------------------------+ V + x
 *                      |  __kernel_virt_end__ = .                  |
 *                      |                                           |
 *                      | __kernel_bss_end = .                      |
 *                      | SECTION .bss                              |
 *                      | __kernel_bss_start__ = .                  |
 *                      |                                           |
 *                      | __kernel_data_end__ = .                   |
 *                      | SECTION .data                             |
 *                      | SECTION .kmodules - internal modules      |
 *                      | __kernel_data_start__ = .                 |
 *                      |                                           |
 *                      | __kernel_rodata_end__ = .                 |
 *                      | SECTION .idt - kernel IDT                 |
 *                      | SECTION .ksym - kernel symbol list        |
 *                      | SECTION .rodata                           |
 *                      | __kernel_rodata_start__ = .               |
 *                      |                                           |
 *                      | __kernel_text_end__ = .                   |
 *                      | SECTION .text                             |
 *                      | __kernel_text_start__ = .                 |
 *                      |                                           |
 *                      | __kernel_virt_start__ = .                 |
 * KERNEL_CODE_START    +-------------------------------------------+ 0xF0400000
 *                      |                                           |
 *                      |    (reserved for kernel page structs)     |
 *                      |                                           |
 *            + 1Mbytes +-------------------------------------------+ 0xF0100000
 *                      |                                           |
 *                      |               (reserved)                  |
 *                      |                                           |
 * KERNEL_VIRTUAL_BASE  +-------------------------------------------+ 0xF0000000
 *                      |                                           |
 *                      |                 . . .                     |
 *                      |                                           |
 *                      +-------------------------------------------+
 *                      |              (kernel stack)               | \
 *                      |              V grows down V               |  |
 *                      |                                           |  | 4092 KB
 *                      |                                           |  |
 *                      |                                           | /
 *                      +-------------------------------------------+
 *                      |                                           | \
 *                      |       (unallocated protection page)       |  | 4 KB
 *                      |                                           | /
 *                      +-------------------------------------------+ V + x
 *                      | __driver_virt_end__ = .                   |
 *                      |                                           |
 *                      | __driver_bss_end__ = .                    |
 *                      | SECTION .bss                              |
 *                      | __driver_bss_start__ = .                  |
 *                      |                                           |
 *                      | __driver_data_end__ = .                   |
 *                      | SECTION .data                             |
 *                      | __driver_data_start__ = .                 |
 *                      |                                           |
 *                      | __driver_rodata_end__ = .                 |
 *                      | SECTION .ksym - driver symbol list        |
 *                      | SECTION .rodata                           |
 *                      | __driver_rodata_start__ = .               |
 *                      |                                           |
 *                      | __driver_text_end__ = .                   |
 *                      | SECTION .text                             |
 *                      | __driver_text_start__ = .                 |
 *                      |                                           |
 *                      | __driver_virt_start__ = .                 |
 * DRIVER_VIRTUAL_BASE  +-------------------------------------------+ 0xD0000000
 *                      |                                           |
 *                      |                 . . .                     |
 *                      |                                           |
 *                      +-------------------------------------------+
 *                      |                                           |
 *                      |       (unallocated protection page)       |
 *                      |                                           |
 * USER_STACK_START     +-------------------------------------------+ ^- 4 KB
 *         page_aligned | SECTION .bss                              |
 *                      |                v grows down v             |
 *                      |                                           |
 *                      |       (unallocated reserved pages)        |
 *                      |                                           |
 *                      |                 ^ grows up ^              |
 *         page_aligned | SECTION .data                             |
 *                      |                                           |
 *         page_aligned | SECTION .rodata                           |
 *                      |                                           |
 *                      | SECTION .text                             |
 * USER_VIRTUAL_START   +-------------------------------------------+ 0x00800000
 *                      |               (reserved)                  |
 *                      +-------------------------------------------+
 *
 */

/** Kernel is loaded at 4 MB mark in physical memory */
#    define KERNEL_PHYSICAL_BASE    0x00400000

/** All virtual addresses above this is reserved for kernel */
#    define KERNEL_VIRTUAL_BASE     0xF0000000

#    define KERNEL_CODE_START       (KERNEL_VIRTUAL_BASE + KERNEL_PHYSICAL_BASE)

#    define KERNEL_STACK_START      0xF0800000

#    define KERNEL_POOL_START       0xF0800000
#    define KERNEL_INITIAL_POOL_SZ  0x00400000

#    define KERNEL_PROTECTION_PAGE1 0xFFFFE000
#    define KERNEL_PROTECTION_PAGE2 KERNEL_STACK_END

#    define USER_VIRTUAL_START      0x00800000

/*
 * Global Descriptor Table:
 *
 *
 * Null segment             +---------------------------------------+
 *                          |                                       |
 * Kernel code segment      +---------------------------------------+
 *                          |                                       |
 * Kernel data segment      +---------------------------------------+
 *                          |                                       |
 * User code segment        +---------------------------------------+
 *                          |                                       |
 * User data segment        +---------------------------------------+
 *                          |                                       |
 * Ring0-ring3 switch TSS   +---------------------------------------+
 *                          |                                       |
 * Kernel call gate         +---------------------------------------+
 *                          |                                       |
 * Kernel double fault TSS  +---------------------------------------+
 *                          |                                       |
 *                          +---------------------------------------+
 *
 */

/** Size of segmentation structure entry in bytes */
#    define SEGMENT_ENTRY_SIZE          8

/** Count of used segments */
#    define SEGMENT_COUNT               8

/** Size of segmentation structure */
#    define SEGMENT_SIZE                (SEGMENT_ENTRY_SIZE * SEGMENT_COUNT)

/** Global Descriptor Table's kernel code segment number */
#    define GDT_KERNEL_CS               1
/** Global Descriptor Table's kernel data segment number */
#    define GDT_KERNEL_DS               2
/** Global Descriptor Table's user code segment number */
#    define GDT_CS                      3
/** Global Descriptor Table's user data segment number */
#    define GDT_DS                      4
/** Task Gate for Ring0 to Ring3 switches */
#    define GDT_TSS_3                   5
/** Call Gate for far calls */
#    define GDT_SEGSYSCALL              6
/** Task Gate for Double Faults */
#    define GDT_TSS_DF                  7

/** Segmentation structure's offsets @{ */
#    define KERNEL_CS                   (GDT_KERNEL_CS  * SEGMENT_ENTRY_SIZE)
#    define KERNEL_DS                   (GDT_KERNEL_DS  * SEGMENT_ENTRY_SIZE)
#    define USER_CS                     (GDT_CS         * SEGMENT_ENTRY_SIZE)
#    define USER_DS                     (GDT_DS         * SEGMENT_ENTRY_SIZE)
#    define TSS_3                       (GDT_TSS_3      * SEGMENT_ENTRY_SIZE)
#    define SEGSYSCALL                  (GDT_SEGSYSCALL * SEGMENT_ENTRY_SIZE)
#    define TSS_DF                      (GDT_TSS_DF     * SEGMENT_ENTRY_SIZE)
/** @} */

/** Interrupt count */
#    define INTERRUPT_COUNT             256

#    define IRQ_OFFSET_MASTER           0x20
#    define IRQ_OFFSET_SLAVE            0x28

/** Specially handled interrupt vectors */
#    define IRQ_ISR_VECTORS             47

/** Kernel context switch frequency in HZ */
#    define KERNEL_FREQ                 100

/** Frame types: 4 MB, 4 KB */
#    define FRAME_SIZE1                 (4 * 1024)
#    define FRAME_SIZE2                 (4 * 1024 * 1024)
#    define FRAME_TYPE_COUNT            2
#    define SUPPORTED_MEMORY_AMOUNT     (4 * 1024 * 1024 * 1024)

#endif /* __flos_x86__config_h__ */
