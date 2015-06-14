/**
 * @file    drivers/bga/bga.h
 * @brief   Bochs Graphics Adapter
 * @version 0.1
 * @date    14.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__BGA__BGA_H__
#    define __flos__BGA__BGA_H__

#    define BGA_VERSION1                0xB0C0
#    define BGA_VERSION2                0xB0C1
#    define BGA_VERSION3                0xB0C2
#    define BGA_VERSION4                0xB0C3
#    define BGA_VERSION5                0xB0C4
#    define BGA_VERSION6                0xB0C5

void bga_set_video_mode(unsigned int width, unsigned int height,
                        unsigned int depth, bool use_linear_fb, bool clear_mem);

void bga_get_video_mode(unsigned int *width, unsigned int *height,
                        unsigned int *depth);

#endif /* __flos__BGA__BGA_H__ */
