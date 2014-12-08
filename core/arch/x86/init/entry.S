/**
 * @file    arch/x86/init/entry.asmx
 * @brief   x86 entry point
 * @version 1.0
 * @date    27.11.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#define __ASSEMBLER__ 1

#include <flos/config.h>

#ifdef __MULTIBOOT__
    #include <multiboot.h>

    #ifdef __ELF__
        #define MULTIBOOT_HEADER_FLAGS       0x00000007
    #else
        #define MULTIBOOT_HEADER_FLAGS       0x00010007
    #endif
#endif /* __MULTIBOOT__ */

[BITS 32]

section .text
global start

start:
    jmp _start

align 4

#ifdef __MULTIBOOT__
multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd - (MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
#   ifndef __ELF__
    dd multiboot_header
    dd _start
    dd _edata
    dd _end
    dd entry
#   endif /* __ELF__ */
#endif /* __MULTIBOOT__ */

_start:
