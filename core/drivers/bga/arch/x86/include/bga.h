/**
 * @file    drivers/bga/arch/x86/bga.h
 * @brief   Bochs Graphics Adapter for x86
 * @version 0.1
 * @date    14.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__BGA__BGA_H__
#    define __flos_x86__BGA__BGA_H__

#    include <flos/types.h>
#    include <flos/arch/io.h>
#    include <vbe.h>

/**
 * Write BGA registers
 * @param index index register
 * @param data  data
 */
static inline void bga_write(u16 index, u16 data) {
    outw(VBE_DISPI_IOPORT_INDEX, index);
    outw(VBE_DISPI_IOPORT_DATA, data);
}

/**
 * Read BGA registers
 * @param  index index register
 * @return       data
 */
static inline u16 bga_read(u16 index) {
    outw(VBE_DISPI_IOPORT_INDEX, index);
    return inw(VBE_DISPI_IOPORT_DATA);
}

/**
 * Gets BGA version
 * @return One of BGA_VERSION* values
 */
static inline u16 bga_version() {
    return bga_read(VBE_DISPI_INDEX_ID);
}

static inline void bga_disable() {
    bga_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
}

static inline void bga_enable() {
    bga_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED);
}

#endif /* __flos_x86__BGA__BGA_H__ */
