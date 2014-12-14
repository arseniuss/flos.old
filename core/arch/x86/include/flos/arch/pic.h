/**
 * @file    arch/x86/include/flos/pm/pic.h
 * @brief   8259 PIC
 * @version 0.1
 * @date    01.05.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__PIC_H__
#    define __flos_x86__PIC_H__

#    include <flos/arch/io.h>

#    define PIC1        0x20    /* IO base address for master PIC */
#    define PIC2        0xA0    /* IO base address for slave PIC */

#    define PIC1_CMD    PIC1
#    define PIC1_DATA   (PIC1 + 1)
#    define PIC2_CMD    PIC2
#    define PIC2_DATA   (PIC2 + 1)

#    define PIC_EOI     0x20    /* End-of-interrupt command */

#    define MAX_IRQS    16
#    define IRQ_SLAVE   0x02    /* IRQ at which slave connects to master */

static inline void pic_eoi(unsigned char irq) {
    if(irq < 16) { /* Ignore invalid */
        if(irq >= 8)
            outb(PIC2_CMD, PIC_EOI);
        outb(PIC1_CMD, PIC_EOI);
    }
}

#    define ICW1_ICW4       0x01    /* ICW4 (not) needed */
#    define ICW1_SINGLE     0x02    /* Single (cascade) mode */
#    define ICW1_INTERVAL4  0x04    /* Call address interval 4 (8) */
#    define ICW1_LEVEL      0x08    /* Level triggered (edge) mode */
#    define ICW1_INIT       0x10    /* Initialization - required! */

#    define ICW4_8086       0x01    /* 8086/88 (MCS-80/85) mode */
#    define ICW4_AUTO       0x02    /* Auto (normal) EOI */
#    define ICW4_MASTER     0x04    /* Master (slave) PIC */
#    define ICW4_BUFFERED   0x08    /* Buffered mode */
#    define ICW4_SFNM       0x10    /* Special fully nested mode (not) */

#    define OCW3_READ_IRR   0x02    /* Read Interrupt Request Register */
#    define OCW3_READ_ISR   0x03    /* Read In-Service Register */
#    define OCW3_POLING     0x04    /* Polling mode (not) */
#    define OCW3_OCW3       0x08

static inline void pic_remap(int off1, int off2) {
    /* ICW1: Starts initialization sequence (in cascade mode) */
    outb(PIC1_CMD, ICW1_INIT + ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT + ICW1_ICW4);

    /* ICW2: Sets IRQ offsets */
    outb(PIC1_DATA, off1);
    outb(PIC2_DATA, off2);

    /* ICW3: tell Master PIC that there is Slave PIC at IRQ2 */
    outb(PIC1_DATA, 1 << IRQ_SLAVE);
    /* ICW3: tell Slave PIC that its cascade identity */
    outb(PIC2_DATA, 2);

    /* ICW4: */
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /* OCW1: Set mask */
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);
}

static inline void pic_set_mask(u16 mask) {
    outb(PIC1_DATA, mask & 0xFF);
    outb(PIC2_DATA, mask >> 8);
}

static inline u16 pic_get_mask() {
    u8 master = inb(PIC1_DATA);
    u8 slave = inb(PIC2_DATA);

    return master | (slave << 8);
}

static inline void pic_disable() {
    /* Mask all interrupts */
    pic_set_mask(0xFFFF);
}

static inline u16 pic_get_irr() {
    outb(PIC1_CMD, OCW3_OCW3 | OCW3_READ_IRR);
    outb(PIC2_CMD, OCW3_OCW3 | OCW3_READ_IRR);

    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

static inline u16 pic_get_isr() {
    outb(PIC1_CMD, OCW3_OCW3 | OCW3_READ_ISR);
    outb(PIC2_CMD, OCW3_OCW3 | OCW3_READ_ISR);

    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

#endif /* __flos_x86__PIC_H__ */