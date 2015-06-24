/**
 * @file    arch/x86/module/init.c
 * @brief   Module initialization
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/assert.h>
#include <flos/module.h>
#include <flos/kprintf.h>
#include <flos/types.h>

extern char __kernel_modules_start__;
extern char __kernel_modules_end__;

struct module **kernel_modules_start =
    (struct module **)&__kernel_modules_start__;
struct module **kernel_modules_end = (struct module **)&__kernel_modules_end__;

void init_modules() {
    int module_count = 0;
    int modules_failed = 0;
    int modules_inited = 0;

    kinfof("Initing kernel static modules ...\n");

    assert(kernel_modules_start != NULL,
           "Error: kernel module start position not found!");

    kdebugf("Kernel modules = {[%p .. %p]}\n",
            kernel_modules_start, kernel_modules_end);

    for(struct module ** m = kernel_modules_start; m < kernel_modules_end; ++m) {
        module_count++;
        register_module(*m);
    }

    for(struct module ** m = kernel_modules_start; m < kernel_modules_end; ++m) {
        kdebugf("Module @ %p [name '%s', init @ %p, exit @ %p]\n",
                *m, (*m)->name, (*m)->init, (*m)->exit);

        if(module_init(*m)) {
            kerrorf("Can't init module %s!\n", (*m)->name);
            modules_failed++;
        } else
            modules_inited++;
    }

    kdebugf("Inited %d modules\n", modules_inited);
    kdebugf("Failed %d modules\n", modules_failed);


    kinfof("OK\n");
}
