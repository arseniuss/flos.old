/**
 * @file    drivers/cmos/cmos.h
 * @brief   CMOS
 * @version 0.1
 * @date    10.10.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__CMOS__CMOS_H__
#    define __flos__CMOS__CMOS_H__

#    include <flos/arch/io.h>
#    include <flos/interrupt.h>

#    define CMOS_PORT1        0x70
#    define CMOS_PORT2        0x71

#    define CMOS_RTC_SEC      0x00
#    define CMOS_RTC_ASEC     0x01
#    define CMOS_RTC_MIN      0x02
#    define CMOS_RTC_AMIN     0x03
#    define CMOS_RTC_HRS      0x04
#    define CMOS_RTC_AHRS     0x05
#    define CMOS_RTC_DOW      0x06
#    define CMOS_RTC_DAY      0x07
#    define CMOS_RTC_MON      0x08
#    define CMOS_RTC_YR       0x09

#    define CMOS_SREG_A       0x0A
#    define CMOS_SREG_B       0x0B
#    define CMOS_SREG_C       0x0C
#    define CMOS_SREG_D       0x0D
#    define CMOS_DIAG         0x0E
#    define CMOS_SHUT_REG     0x0F

#    define CMOS_DRV_FLS      0x10
#    define CMOS_SYS_CFG      0x11
#    define CMOS_DRV_HDD      0x12
#    define CMOS_TYPE_P       0x13
#    define CMOS_EQUIP        0x14

// Status register B {
#    define CMOS_SREG_B_DLS   (0x1 << 1)    // Daylight saving time
#    define CMOS_SREG_B_12H   (0x1 << 2)    // 12 hour mode (else 24 hour)
#    define CMOS_SREG_B_SWF   (0x1 << 3)    // Status register A square wave freq
#    define CMOS_SREG_B_UEI   (0x1 << 4)    // Update Ended interrupt
#    define CMOS_SREG_B_AI    (0x1 << 5)    // Alarm interrupt
#    define CMOS_SREG_B_PI    (0x1 << 6)    // Periodic interrupt
#    define CMOS_SREG_B_CUC   (0x1 << 7)    // Clock update cycle
// }

// Statuc register C {
#    define CMOS_SREG_C_UF    (0x1 << 4)
#    define CMOS_SREG_C_AF    (0x1 << 5)
#    define CMOS_SREG_C_PF    (0x1 << 6)
#    define CMOS_SREG_C_IRQF  (0x1 << 7)
// }


static inline u8 cmos_read(u8 reg) {
    disable_interrupts();
    outb(CMOS_PORT1, 0x80 | reg);
    u8 val = inb(CMOS_PORT2);
    enable_interrupts();
    return val;
}

static inline void cmos_write(u8 reg, u8 val) {
    disable_interrupts();
    outb(CMOS_PORT1, 0x80 | reg);
    outb(CMOS_PORT2, val);
    enable_interrupts();
}

#endif
