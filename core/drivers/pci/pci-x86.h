/**
 * @file    ddrivers/pci/pci-x86.h
 * @brief   Peripheral Component Interconnect for x86
 * @version 0.1
 * @date    16.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__PCI__PCI_H__
#    define __flos_x86__PCI__PCI_H__

#    include <flos/arch/io.h>
#    include <flos/types.h>
#    include <flos/kprintf.h>

#    define CONFIG_ADDRESS          0xCF8
#    define CONFIG_DATA             0xCFC

static inline u16 pci_read(u8 bus, u8 slot, u8 func, u8 off) {
    u32 address;

    address =
        (u32) ((bus << 16) | (slot << 11) | (func << 8) | (off & 0xFC) |
               0x80000000);

    outl(CONFIG_ADDRESS, address);

    return (inl(CONFIG_DATA) >> ((off & 2) * 8)) & 0xffff;
}

#endif /* __flos_x86__PCI__PCI_H__ */
