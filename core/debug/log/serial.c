/**
 * @file    debug/log/serial.c    
 * @brief   Logging to serial port
 * @version 1.0
 * @date    14.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/arch/io.h>
#include <flos/types.h>

#include "../log.h"

#define COM1PORT            0x3F8

const char *sh_color[] = {
                          "\e[0m", ///< OK
                          "\e[32m", ///< INFO
                          "\e[33m", ///< WARN
                          "\e[31m", ///< ERR
                          "\e[31m" ///< PANIC
};
int sh_colors = {sizeof (sh_color) / sizeof (sh_color[0])};

int serial_puts(const char *s);

/**
 * Checks of serial port if free to write
 * @param port Port number
 */
static bool serial_isempty(u16 port) {
    return (bool) (inb(port + 5) & 0x20);
}

void serial_putc(char ch) {
#ifdef CONFIG_BOCHS
    /* If we emulate in Bochs, use E9 hack. */
    outb(BOCHS_PORT, ch);
#endif

    while(serial_isempty(COM1PORT) == 0);
    outb(COM1PORT, ch);
}

static void change_color(unsigned char no) {
    for(const char *s = sh_color[no]; *s; s++)
        serial_putc(*s);
}

int serial_puts(const char *s) {
    int len = 0;

    for(; *s; s++, len++) {
        if(*s < sh_colors) {
            change_color(*s);
        } else
            serial_putc(*s);
    }
    change_color(0);

    return len;
}

log_puts_t *kernel_log_handle = &serial_puts;
