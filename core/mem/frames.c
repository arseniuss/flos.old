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
#include <flos/mem/phys.h>
#include <flos/assert.h>
#include <flos/string.h>

#ifndef DEBUG_FRAMES
#    undef kdebugf
#    define kdebugf(...)
#endif

struct frame *root = NULL;

/**
 * Allocate frame
 * @param  size_no size type number
 * @return         allocated frame physical address or -1
 */
addr_t frame_alloc(int size_no) {
    addr_t idxes[FRAME_TYPE_COUNT], ret = 0;
    int entry = -1, old_entry = -1, lvl = size_no;
    struct frame *pos = root;

    assert(size_no < FRAME_TYPE_COUNT);

    memset(idxes, 0, sizeof(idxes));

    while(lvl) {
        kdebugf("lvl = %d\n", FRAME_TYPE_COUNT - lvl);

        if((entry = bitmap_first_free(&pos->bitmap)) == -1) {
            kdebugf("No free bit in lvl %d\n", FRAME_TYPE_COUNT - lvl);

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
            kdebugf("Found free bit in lvl %d\n", FRAME_TYPE_COUNT - lvl);

            pos = (*pos->frames)[entry];
            lvl--;
            idxes[lvl] = entry;
            old_entry = entry;
        }
    }

    entry = bitmap_first_free(&pos->bitmap);
    idxes[lvl] = entry;
    bitmap_set(&pos->bitmap, entry);

    for(int i = 0; i < FRAME_TYPE_COUNT; ++i) {
        kdebugf("lvl %d idx %d sz %d\n", FRAME_TYPE_COUNT - i - 1, idxes[i],
                frame_sizes[FRAME_TYPE_COUNT - i - 1]);
        ret |= idxes[i] * frame_sizes[FRAME_TYPE_COUNT - i - 1];
    }

    return ret;
}

void frame_set(addr_t start, size_t length) {
    size_t counts[FRAME_TYPE_COUNT];

    for(int i = 0; i < FRAME_TYPE_COUNT; ++i) {
        counts[i] = length / frame_sizes[i];

        kdebugf("Setting %d of %d size frames\n", counts[i], frame_sizes[i]);

        for(int j = 0; j < counts[i]; ++j) {
            frame_sz_set(start + j * frame_sizes[i], i);
            counts[i] -= 1;
            if(i + 1 < FRAME_TYPE_COUNT)
                counts[i + 1] -= frame_sizes[i] / frame_sizes[i + 1];
        }
    }

    for(int i = 0; i < FRAME_TYPE_COUNT; ++i)
        kdebugf("frame %d set %d\n", i, counts[i]);
}

void frame_sz_set(addr_t start, int size_no) {
    assert(size_no < FRAME_TYPE_COUNT);
    assert(((start & ~(frame_sizes[size_no] - 1)) == start),
           "%b & %b", start, (frame_sizes[size_no] - 1));

    int idx, off = start;
    struct frame *pos = root;

    for(int i = 0; i <= size_no; ++i) {
        idx = off / frame_sizes[i];
        off = off % frame_sizes[i];

        if(i == size_no) {
            bitmap_set(&pos->bitmap, idx);

            break;
        }

        pos = (*pos->frames)[idx];
    }
}

void frame_free(addr_t addr, int size_no) {

}
