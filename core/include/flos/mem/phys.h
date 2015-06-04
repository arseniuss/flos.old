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

/**
 * Allocate free physical memory frame
 * @param  size_no size type number
 * @return         address of physical frame
 */
__linkage addr_t frame_alloc(int size_no);

/**
 * Set frames from physical address as used
 * @param start  physical address start
 * @param length block length
 */
__linkage void frame_set(addr_t start, size_t length);

__linkage void frame_sz_set(addr_t start, int size_no);


#endif /* __flos__MEM_PHYS_H__ */
