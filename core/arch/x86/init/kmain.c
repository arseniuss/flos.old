/**
 * @file    arch/x86/init/kmain.c
 * @brief   x86 kernel main entry
 * @version 1.0
 * @date    28.11.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/arch/descriptors.h>
#include <flos/defs.h>
#include <flos/kernel.h>
#include <flos/string.h>

void log_header(const char *fmt, ...) {
    char buf[MAX_CHAR_BUF_LEN];
    int pad;
    va_list args;


    va_start(args, fmt);
    vsnprintf(buf, MAX_CHAR_BUF_LEN, fmt, args);
    va_end(args);

    pad = (strlen(buf) >= 60) ? 0 : (60 - strlen(buf)) / 2;
    kinfof("%*.*s%s", pad, pad, " ", buf);
}

int kmain() {
    log_header("flos kernel v%d.%d [%d] %s\n", KERNEL_MAJOR, KERNEL_MINOR,
               KERNEL_PATCH, KERNEL_NAME);
    log_header("Compiled @ %s %s\n", __DATE__, __TIME__);
    kinfof("============================================================\n");

    init_idt();

    init_timer();

    return 0;
}
