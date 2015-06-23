/**
 * @file    ddrivers/pci/pci-dev.h
 * @brief   Peripheral Component Interconnect devices
 * @version 0.1
 * @date    23.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__PCI__PCI_DEV_H__
#    define __flos__PCI__PCI_DEV_H__

#    include <flos/list.h>
#    include <flos/types.h>

struct pci_bus {
    u8 no;                      // bus number [0 .. 256]

    struct pci_bus *parent;
    struct list_head devices;

    struct list_head __pci_root;
};

struct pci_dev {

    struct pci_bus *bus;

    struct list_head __devices; // pci_bus.devices
};

extern struct list_head pci_root;

void pci_scan();

#endif /* __flos__PCI__PCI_DEV_H__ */
