/**
 * @file    drivers/pci/pci-dev.c
 * @brief   Peripheral Component Interconnect devices
 * @version 1.0
 * @date    23.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>
#include <flos/kernel.h>
#include <flos/kprintf.h>

#include "pci.h"
#include "pci-dev.h"

#define pci_vendor(bus, dev, func)  pci_read(bus, dev, 0, offsetof(struct pci_cfg, vendor_id))
#define pci_header(bus, dev, func)  pci_read(bus, dev, 0, offsetof(struct pci_cfg, header_type))

LIST_HEAD(pci_root);

struct pci_bus *pci_bus_create(u8 bus) {
    struct pci_bus *pci_bus = kmalloc(sizeof(struct pci_bus), KMALLOC_ZERO);

    pci_bus->no = bus;
    INIT_LIST_HEAD(&pci_bus->devices);

    list_add_tail(&pci_bus->__pci_root, &pci_root);

    return pci_bus;
}

void pci_dev_create(struct pci_bus *pci_bus, u8 bus, u8 dev, u8 func) {
    struct pci_dev *pci_dev = kmalloc(sizeof(struct pci_dev), KMALLOC_ZERO);

    pci_dev->bus = pci_bus;

    list_add_tail(&pci_bus->devices, &pci_dev->__devices);
}

void pci_scan() {
    struct pci_bus *pci_bus = NULL;

    for(int bus = 0; bus < 256; bus++) {
        for(int dev = 0; dev < 32; dev++) {
            if(pci_vendor(bus, dev, 0) != INVALID_VENDOR_ID) {
                if(!pci_bus)
                    pci_bus = pci_bus_create(bus);

                pci_dev_create(pci_bus, bus, dev, 0);

                if((pci_header(bus, dev, 0) & HEADER_MULTIFUNCTION) != 0) {
                    for(int func = 1; func < 8; func++) {
                        if(pci_vendor(bus, dev, func) != INVALID_VENDOR_ID) {
                            pci_dev_create(pci_bus, bus, dev, func);
                        }
                    }
                }
            }
        }

        pci_bus = NULL;
    }
}
