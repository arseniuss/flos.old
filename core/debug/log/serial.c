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
#include <flos/kprintf.h>

#define COM1PORT            0x3F8

const char *sh_color[] = {
    "\e[1\e[31m",                      ///< ENERG
    "\e[31m",                          ///< ALERT
    "\e[91m",                          ///< CRIT
    "\e[31m",                          ///< ERR
    "\e[33m",                          ///< WARN
    "\e[92m",                          ///< NOTICE
    "\e[32m",                          ///< INFO
    "\e[94m",                          ///< DEBUG
};
int sh_colors = { sizeof(sh_color) / sizeof(sh_color[0]) };

/**
 * Checks of serial port if free to write
 * @param port Port number
 */
static bool serial_isempty(u16 port) {
    return (bool) (inb(port + 5) & 0x20);
}

void serial_putc(char ch) {
#ifdef CONFIG_BOCHS
    /*
     * If we emulate in Bochs, use E9 hack. 
     */
    outb(0xE9, ch);
#endif

    while(serial_isempty(COM1PORT) == 0);
    outb(COM1PORT, ch);
}

static void change_color(int no) {
    for(const char *s = sh_color[no]; *s; s++)
        serial_putc(*s);
}

static void clear_color() {
    const char *s = "\e[0m";

    for(; *s; s++)
        serial_putc(*s);
}

int serial_puts(int level, const char *s) {
    int len = 0;

    change_color(level);

    for(; *s; s++, len++) {
        serial_putc(*s);
    }
    clear_color();

    return len;
}
