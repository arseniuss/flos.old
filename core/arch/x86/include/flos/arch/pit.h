/**
 * @file    arch/x86/include/flos/pm/pit.h
 * @brief   8253/8254 PIT
 * @version 0.1
 * @date    30.01.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__PIC_H__
#    define __flos_x86__PIC_H__

#    include <arch/io.h>

#    define PIT_BASE_PORT       0x40

#    define PIT_CHANNEL0        (PIT_BASE_PORT + 0)
#    define PIT_CHANNEL1        (PIT_BASE_PORT + 1)
#    define PIT_CHANNEL2        (PIT_BASE_PORT + 2)
#    define PIT_CMD             (PIT_BASE_PORT + 3)

/** BCD operating mode */
#    define PIT_BCD_MODE        0x01

/** Operating mode 0: Interrupt On Terminal Count */
#    define PIT_OP_MODE_0       0x00
/** Operating mode 1: Hardware Re-triggerable One-shot */
#    define PIT_OP_MODE_1       0x02
/** Operating mode 2: Rate Generator */
#    define PIT_OP_MODE_2       0x04
/** Operating mode 3: Square Wave Generator */
#    define PIT_OP_MODE_3       0x06
/** Operating mode 4: Software Triggered Strobe */
#    define PIT_OP_MODE_4       0x08
/** Operating mode 5: Hardware Triggered Strobe */
#    define PIT_OP_MODE_5       0x0A

/** Access mode @{ */
#    define PIT_AC_MODE_VALUE   0x00
#    define PIT_AC_MODE_LOBYTE  0x10
#    define PIT_AC_MODE_HIBYTE  0x20
#    define PIT_AC_MODE_LOHI    0x30
/** @} */

/** Select channel @{ */
#    define PIT_SEL_CH0         0x00
#    define PIT_SEL_CH1         0x40
#    define PIT_SEL_CH2         0x80
#    define PIT_SEL_READBACK    0xC0
/** @} */

#    define PIT_FREQ            1193182

static inline void pit_timer_phase(int hz) {
    int divisor = PIT_FREQ / hz;

    // Square wave mode, Low and Hi bytes
    outb(PIT_CMD, PIT_OP_MODE_3 | PIT_AC_MODE_LOHI);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, divisor >> 8);
}

#endif /* __flos_x86__PIC_H__ */
