/**
 * @file    include/video/vbe.h
 * @brief   VESA BIOS Extensions
 * @version 0.1
 * @date    14.06.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__VBE_H__
#    define __flos__VBE_H__

#    define VBE_DISPI_IOPORT_INDEX          0x01CE
#    define VBE_DISPI_IOPORT_DATA           0x01CF
#    define VBE_DISPI_INDEX_ID              0x0
#    define VBE_DISPI_INDEX_XRES            0x1
#    define VBE_DISPI_INDEX_YRES            0x2
#    define VBE_DISPI_INDEX_BPP             0x3
#    define VBE_DISPI_INDEX_ENABLE          0x4
#    define VBE_DISPI_INDEX_BANK            0x5
#    define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#    define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#    define VBE_DISPI_INDEX_X_OFFSET        0x8
#    define VBE_DISPI_INDEX_Y_OFFSET        0x9

#    define VBE_DISPI_DISABLED              0x00
#    define VBE_DISPI_ENABLED               0x01
#    define VBE_DISPI_GETCAPS               0x02
#    define VBE_DISPI_8BIT_DAC              0x20
#    define VBE_DISPI_LFB_ENABLED           0x40
#    define VBE_DISPI_NOCLEARMEM            0x80

#    define VBE_DISPI_BPP_4                 0x04
#    define VBE_DISPI_BPP_8                 0x08
#    define VBE_DISPI_BPP_15                0x0F
#    define VBE_DISPI_BPP_16                0x10
#    define VBE_DISPI_BPP_24                0x18
#    define VBE_DISPI_BPP_32                0x20

#endif /* __flos__VBE_H__ */
