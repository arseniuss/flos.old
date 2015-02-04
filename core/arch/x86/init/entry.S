/**
 * @file    arch/x86/init/entry.S
 * @brief   x86 entry point
 * @version 1.0
 * @date    27.11.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#define __PREPROCESSING__ 1

#include <flos/arch/registers.h>
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
    cli
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
    /* load gdt table */
    lgdt [kernel_gdt_ptr - KERNEL_VIRTUAL_BASE]

    /* Check if 4 MB pages is supported */
    mov eax, 0
    cpuid
    test edx, 0x4
    jz .unsupported
.supported:
    /* 4MB pages is supported */

    /* Enable page size extenstions (4MB pages) */
    mov ecx, cr4
    or ecx, CR4_PSE
    mov cr4, ecx

    /* Map kernel pages */
    mov eax, 0x83
    mov ebx, (KERNEL_PHYSICAL_BASE | 0x83)
    mov edx, kernel_page_directory - KERNEL_VIRTUAL_BASE
    mov dword [edx], eax
    mov dword [edx + 4], ebx
    mov edx, kernel_page_directory + (KERNEL_VIRTUAL_BASE >> 22) * 4 - KERNEL_VIRTUAL_BASE
    mov dword [edx], eax
    mov dword [edx + 4], ebx
    jmp .1
.unsupported:
    /* 4MB pages is not supported */
    mov eax, (kernel_page_directory.table1 - KERNEL_VIRTUAL_BASE)
    or eax, 0x3
    mov ebx, (kernel_page_directory.table2 - KERNEL_VIRTUAL_BASE)
    or ebx, 0x3

    mov edx, kernel_page_directory - KERNEL_VIRTUAL_BASE
    mov ecx, (KERNEL_VIRTUAL_BASE >> 22) * 4
    mov dword [kernel_page_directory - KERNEL_VIRTUAL_BASE], eax
    mov dword [edx + 4], ebx
    mov dword [edx + ecx], eax
    mov dword [edx + ecx + 4], ebx

    /* Fill page tables */
    mov eax, 0
    mov ebx, 0
.filltable:
    mov ebx, 4096
    imul ebx, eax
    or ebx, 0x3
    mov edx, kernel_page_directory.table1 - KERNEL_VIRTUAL_BASE
    mov dword [edx + eax * 4], ebx
    mov edx, kernel_page_directory.table2 - KERNEL_VIRTUAL_BASE
    add ebx, 0x00400000
    mov dword [edx + eax * 4], ebx

    add eax, 1
    cmp eax, 1024
    jz .1
    jmp .filltable
.1:
    /* Load page directory */
    mov ecx, kernel_page_directory - KERNEL_VIRTUAL_BASE
    mov cr3, ecx

    /* Enable paging */
    mov ecx, cr0
    or ecx, CR0_PG
    mov cr0, ecx

/* Change selectors */
    mov ax, KERNEL_DS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp KERNEL_CS:_higher_start

_higher_start:
extern kmain
    /* Since stack borders memory pool offset is needed to avoid overflow */
    mov esp, (KERNEL_STACK_START - 8)

#ifdef __TEST__
    extern __kernel_test_ctors_start__
    extern __kernel_test_ctors_end__

    mov  ebx, __kernel_test_ctors_start__
    jmp  .ctor_check
.call_ctor:
    call [ebx]
    add  ebx, 4
.ctor_check:
    cmp  ebx, __kernel_test_ctors_end__
    jb   .call_ctor
#endif

    call kmain

#ifdef __TEST__
    extern __kernel_test_dtors_start__
    extern __kernel_test_dtors_end__

    mov  ebx, __kernel_test_dtors_start__
    jmp  .dtor_check
.call_dtor:
    call [ebx]
    add  ebx, 4
.dtor_check:
    cmp  ebx, __kernel_test_dtors_end__
    jb   .call_dtor
#endif

    jmp loop

    mov ax, (USER_DS | 0x3)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push (USER_DS | 0x3)
    push eax
    pushf
    push (USER_CS | 0x3)
    push USER_VIRTUAL_START
    iret

loop:
    jmp loop

section .data

kernel_page_directory:
    times 1024 dd 0
    times 1024 dd 0
/* page table mapped to [0x00000000 - 0x00400000] */
.table1:
    times 1024 dd 0
/* page table mapped to [0x00400000 - 0x00800000] */
.table2:
    times 1024 dd 0

kernel_gdt:
; First Segment, null
                dw 0x0000   ;LowLimit
                dw 0x0000   ;LowBase
                db 0x00     ;MiddleBase
                db 0x00     ;Access
                db 0x00     ;Granularity
                db 0x00     ;HighBase
; Second segment, higher half kernel code
                dw 0xFFFF
                dw 0x0000
                db 0x00
                db 0x9A
                db 0xCF
                db 0x00
; Third segment, higher half kernel data
                dw 0xFFFF
                dw 0x0000
                db 0x00
                db 0x92
                db 0xCF
                db 0x00
; Fourth segment, user data
                dw 0xFFFF
                dw 0x0000
                db 0x00
                db 0xFA
                db 0xCF
                db 0x00
; Fifth segment, user code
                dw 0xFFFF
                dw 0x0000
                db 0x00
                db 0xF2
                db 0xCF
                db 0x00
; Other
                times (SEGMENT_COUNT - 5) dq 0

kernel_gdt_ptr:
                dw SEGMENT_SIZE - 1
                dd kernel_gdt

