/**
 * @file    ddrivers/pci/pci.h
 * @brief   Peripheral Component Interconnect
 * @version 0.1
 * @date    15.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__PCI__PCI_H__
#    define __flos__PCI__PCI_H__

#    include <flos/types.h>

#    ifdef __x86__
#        include "pci-x86.h"
#    endif

#    define INVALID_VENDOR_ID       0xFFFF

#    define HEADER_MULTIFUNCTION    0x80

struct pci_cfg {
    u16 vendor_id;
    u16 device_id;
    u16 command;
    u16 status;

    u8 revision_id;
    u8 interface;
    u8 sub_class;
    u8 base_class;

    u8 cache_line_sz;
    u8 latency_timer;
    u8 header_type;
    u8 bist;

    u32 BAR[6];

    u32 cardbus_cis_ptr;
    u16 sub_vendor_id;
    u16 sub_system;
    u32 exp_rom_addr;
    u8 capabilities;
    u8 __reserved[7];
    u8 int_line;
    u8 int_pin;
    u8 min_grant;
    u8 max_laterncy;
};

enum PCI_CLASS {
    MASS_STORAGE_CONTROLLER = 0x01,
    NETWORK_CONTROLLER = 0x02,
    DISPLAY_CONTROLLER = 0x03,
    MULTIMEDIA_CONTROLLER = 0x04,
    MEMORY_CONTROLLER = 0x05,
    BRIDGE_DEVICE = 0x06,
    SIMPLE_COMUNICATION_CONTROLLER = 0x07,
    BASE_SYSTEM_PERIPHERALS = 0x08,
    INPUT_DEVICE = 0x09,
    PROCESSOR = 0x0B,
    UNDEFINED = 0xFF
};


#endif /* __flos__PCI__PCI_H__ */
