/**
 * @file    arch/x86/init/idt.S
 * @brief   Interrupt Descriptor Table
 * @version 0.1
 * @date    01.05.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

section .text

%macro ISR_NOERRCODE 1
global isr%1

isr%1:
    cli
.1:  jmp .1
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1

isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

%macro IRQ 2
global irq%1

irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47


section .data
global idt_vectors
idt_vectors:
    dd isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9
    dd isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19
    dd isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29
    dd isr30, isr31
    dd irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9,
    dd irq10, irq11, irq12, irq13, irq14, irq15

section .text

extern isr_handler

isr_common_stub:
    pusha

    push ds
    push es
    push fs
    push gs
    push ss

    mov ax, 0x10    //change selectors
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp
    call isr_handler

    mov esp, eax

    pop ss
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

extern irq_handler

irq_common_stub:
    pusha

    push ds
    push es
    push fs
    push gs
    push ss

    mov ax, 0x10    //change selectors
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp
    call  irq_handler
    mov esp, eax

    pop ss
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret