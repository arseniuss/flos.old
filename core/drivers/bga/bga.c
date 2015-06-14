/**
 * @file    drivers/bga/bga.c
 * @brief   Bochs Graphics Adapter
 * @version 1.0
 * @date    14.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <vbe.h>
#include <bga.h>

void bga_set_video_mode(unsigned int width, unsigned int height,
                        unsigned int depth, bool use_linear_fb, bool clear_mem) 
{
    bga_disable();
    bga_write(VBE_DISPI_INDEX_XRES, width);
    bga_write(VBE_DISPI_INDEX_YRES, height);
    bga_write(VBE_DISPI_INDEX_BPP, depth);
    bga_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
              (use_linear_fb ? VBE_DISPI_LFB_ENABLED : 0) |
              (clear_mem ? 0 : VBE_DISPI_NOCLEARMEM));
}

void bga_get_video_mode(unsigned int *width, unsigned int *height,
                        unsigned int *depth) {
    bga_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_GETCAPS);

    *width = bga_read(VBE_DISPI_INDEX_XRES);
    *height = bga_read(VBE_DISPI_INDEX_YRES);
    *depth = bga_read(VBE_DISPI_INDEX_BPP);
}
