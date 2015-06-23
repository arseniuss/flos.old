/**
 * @file    arch/x86/module.ld
 * @brief   Linker script for linking module
 * @version 0.1
 * @date    23.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#define __PREPROCESSING__                       1

ENTRY(init)
OUTPUT_FORMAT("elf32-i386")

SECTIONS
{
    module_start = .;

    .text ALIGN(0x1000) :
    {
        *(.text*)
    }

    .data ALIGN(0x1000) :
    {
        *(.data*)
    }

    .module.ctors :
    {
        *(.module.ctor*)
    }

    .module.dtors :
    {
        *(.module.dtor*)
    }

    .rodata ALIGN(0x1000) :
    {
        *(.rodata*)
    }

    module_end = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
    }
}
