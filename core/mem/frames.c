/**
 * @file    mem/frames.c
 * @brief   Physical memory frames
 * @version 1.0
 * @date    23.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

/**
 * This file contains code which represents levels of physical memory frames.
 *
 * For some platforms frame size equals page size, for others only smallest
 * frames is same size as page.
 *
 * Frame size (frame.sz) should be 2^x
 *
 * 4 GB frame structure:
 *    Level 1 bitmap contains 1024 each representing 4 MB page
 *    Level 2 bitmap contains 1024 each representing 4 KB page
 *
 *    SUM: 1024 + 1024^2 bits
 */

#include <flos/types.h>
#include <flos/bitmap.h>
#include <flos/list.h>
#include <flos/kprintf.h>
#include <flos/kernel.h>
#include <flos/config.h>

struct frame {
    addr_t phys_addr;           // physical address of frame
    struct bitmap bitmap;       // bitmap structure
    struct frame *up;           // upper frame

    struct frame *frames[];
};

struct frame *root;
extern size_t frame_sizes[FRAME_TYPE_COUNT];

/**
 * Allocate frame
 * @param  size_no size type number
 * @return         allocated frame physical address or -1
 */
addr_t frame_alloc(int size_no) {
    int entry = -1, old_entry = -1, lvl = size_no;
    struct frame *pos = root;

    while(lvl) {
        if((entry = bitmap_first_free(&pos->bitmap)) == -1) {
            if(pos->up == NULL) {
                kcritf("No free frame on @p!\n", pos->phys_addr);
                return -1;
            }

            /*
             * This frame is full -- mark bit in upper frame 
             */
            if(old_entry != -1)
                bitmap_set(&pos->up->bitmap, old_entry);
            /*
             * Go back to upper frame 
             */
            lvl++;
            pos = pos->up;
        } else {
            pos = pos->frames[entry];
            lvl--;
            old_entry = entry;
        }
    }

    entry = bitmap_first_free(&pos->bitmap);
    bitmap_set(&pos->bitmap, entry);

    return entry;
}

void frame_free(addr_t addr, int size_no) {

}
