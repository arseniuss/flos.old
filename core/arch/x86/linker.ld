/**
 * @file    arch/x86/linker.ld
 * @brief   x86 architecture's linker file
 * @version 1.0
 * @date    21.04.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

/**
 * This is kernel linker map for x86 architectures versions.
 *
 * All kernel sections are page aligned.
 */

#define __PREPROCESSING__                       1
#include <flos/config.h>

ENTRY(start)
OUTPUT_FORMAT("elf32-i386")

SECTIONS {
    . = KERNEL_CODE_START;

    __kernel_start__ = . - KERNEL_VIRTUAL_BASE ;
    __kernel_virt_start__ = .;

    __kernel_text_start__ = .;

    /* code section */
    .text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_VIRTUAL_BASE) {
        /* entry point first! */
    	*.entry.S.o(.text)
        *(.text)
    	*(.gnu.linkonce.t*)
    }

    __kernel_text_end__ = .;

    __kernel_rodata_start__ = .;

    /* read-only data section */
    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_BASE) {
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    /* kernel symbol table section */
    .ksym : AT(ADDR(.ksym) - KERNEL_VIRTUAL_BASE) {

        __kernel_ksymtbl_start__ = .;

        *(SORT(.ksymtab*))

        __kernel_ksymtbl_end__ = .;

        *(SORT(.kstrtab*))
    }

    __kernel_rodata_end__ = .;

    __kernel_data_start__ = .;

    /* kernel module list */
    .kmodules : AT(ADDR(.kmodules) - KERNEL_VIRTUAL_BASE) {

        __kernel_modules_start__ = .;

        *(.kmodules*)

        __kernel_modules_end__ = .;

        *(.kmod*)

    }

    /* readable data section */
    .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VIRTUAL_BASE) {
    	*(.data)
    	*(.gnu.linkonce.d*)
    }

    __kernel_data_end__ = .;

    __kernel_bss_start__ = .;

    /* stack section */
    .bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_VIRTUAL_BASE) SUBALIGN(4096) {
        *(COMMON)
        *(EXCLUDE_FILE(*.entry.S.o) .bss)
	    *.entry.S.o(.bss)
    	*(.gnu.linkonce.b*)
    }

    __kernel_bss_end__ = .;

    __kernel_end__ = . - KERNEL_VIRTUAL_BASE;
    __kernel_virt_end__ = .;

    /DISCARD/ :{
        *(.comment)
    	*(.eh_frame)
    }
}
