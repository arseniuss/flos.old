/**
 * @file    drivers/rtc/rtc-cmos.c
 * @brief   CMOS based RTC
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/defs.h>
#include <flos/interrupt.h>
#include <flos/types.h>
#include <flos/kprintf.h>
#include <flos/module.h>
#include <flos/time.h>

#include "../cmos/cmos.h"

#define bcd2bin(val)         (((val) / 16) * 10 + ((val) & 0xF))

int rtc_init(void);
void rtc_exit(void);

int rtc_update_interrupt(struct iregs *regs);

struct module rtc_module = {
    .name = "rtc",
    .init = &rtc_init,
    .exit = &rtc_exit
};

struct interrupt_handle rtc_upd_handle = {
    .ihandle_func = &rtc_update_interrupt
};

bool update_int_installed = FALSE;

static void rtc_read_data() {
    int sec = bcd2bin(cmos_read(CMOS_RTC_SEC));
    int min = bcd2bin(cmos_read(CMOS_RTC_MIN));
    int hrs = bcd2bin(cmos_read(CMOS_RTC_HRS));

    int day = bcd2bin(cmos_read(CMOS_RTC_DAY));
    int mon = bcd2bin(cmos_read(CMOS_RTC_MON));
    int yrs = bcd2bin(cmos_read(CMOS_RTC_YR)) + 2000;

    s64 unix_time = get_unix_time(yrs, mon, day, hrs, min, sec);

    timestamp = unix_time;
}

int rtc_update_interrupt(struct iregs *regs) {
    if(update_int_installed) {
        u8 creg = cmos_read(CMOS_SREG_C);

        if(has_flag(creg, CMOS_SREG_C_UF)) {
            rtc_read_data();

            return INTERRUPT_HANDLED;
        }
    }

    return INTERRUPT_NOT_HANDLED;
}

int rtc_init(void) {
    kdebugf("Initing RTC ...\n");

    register_interrupt(IRQ(8), &rtc_upd_handle);

    int regb = cmos_read(CMOS_SREG_B);
    cmos_write(CMOS_SREG_B, regb | CMOS_SREG_B_PI);

    update_int_installed = TRUE;

    kdebugf("RTC inited");

    return 0;
}

void rtc_exit(void) {

}

INTERNAL_MODULE(rtc_module);
