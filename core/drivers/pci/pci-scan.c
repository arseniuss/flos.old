/**
 * @file    drivers/pci/pci-scan.c
 * @brief   Peripheral Component Interconnect scanning functions
 * @version 1.0
 * @date    15.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>

void pci_for_each(void (*f) (u8, u8)) {
    for(u8 bus = 0; bus < 256; bus++)
        for(u8 dev = 0; dev < 32; dev++)
            f(bus, dev);
}
