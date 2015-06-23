/**
 * @file    drivers/pci/main.c
 * @brief   BPeripheral Component Interconnect driver
 * @version 1.0
 * @date    15.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>
#include <flos/kprintf.h>

#include "pci-dev.h"

int pci_init() {
    kdebugf("Probing PCI ... \n");

    pci_scan();

    kdebugf("PCI inited\n");

    return 0;
}

void pci_exit() {

}

struct module pci_module = {
    .name = "pci",
    .init = &pci_init,
    .exit = &pci_exit
};

INTERNAL_MODULE(pci_module);
