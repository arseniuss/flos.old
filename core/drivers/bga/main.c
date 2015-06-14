/**
 * @file    drivers/bga/main.c
 * @brief   Bochs Graphics Adapter
 * @version 1.0
 * @date    14.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>
#include <flos/kprintf.h>
#include <flos/types.h>
#include <bga.h>

#include "bga.h"

int bga_init() {
#ifdef __DEBUG__
    u16 version = bga_version();
    unsigned int max_width, max_height, max_bpp;

    kdebugf("BGA version %x\n", version);

    bga_get_video_mode(&max_width, &max_height, &max_bpp);

    kdebugf("Max width %d, max height %d, max depth %d\n",
            max_width, max_height, max_bpp);
#endif

    kinfof("BGA driver inited ...\n");

    bga_set_video_mode(1024, 768, VBE_DISPI_BPP_32, 1, 1);

    return 0;
}

void bga_exit() {

}

struct module bga_module = {
    .name = "bga",
    .init = &bga_init,
    .exit = &bga_exit
};

INTERNAL_MODULE(bga_module);
