/**
 * @file    include/flos/mem/phys.h
 * @brief   Physical memory
 * @version 1.0
 * @date    24.05.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__MEM_PHYS_H__
#    define __flos__MEM_PHYS_H__

#    include <flos/config.h>
#    include <flos/bitmap.h>

/**
 * Physical page frame free/allocated structure
 */
struct frame {
    addr_t phys_addr;           // physical address of frame
    struct bitmap bitmap;       // bitmap structure
    struct frame *up;           // upper frame

    struct frame *(*frames)[];  // pointer to array of pointer to struct frame
};

/**
 * Page frame sizes
 */
extern size_t frame_sizes[FRAME_TYPE_COUNT];

/**
 * Root of physical page frame structure
 */
extern struct frame *root;

/**
 * Gets physical address
 * @param  addr pointer to memory location
 * @return      physical address or 0
 */
addr_t phys(void *addr);


#endif /* __flos__MEM_PHYS_H__ */
