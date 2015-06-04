/**
 * @file    arch/x86/mem/frames.c
 * @brief   x86 physical page frames
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/assert.h>
#include <flos/kernel.h>
#include <flos/config.h>
#include <flos/mem/phys.h>
#include <flos/bitmap.h>
#include <flos/kprintf.h>

#define BITMAP_BIT_COUNT    1024

size_t frame_sizes[FRAME_TYPE_COUNT] = {
    4 * 1024 * 1024,
    4 * 1024
};

void init_frames() {
    //addr_t phys_addr;
    void *bitmap_memory;

    kprintf("Initing physical page frames ... ");

    assert(root == NULL, "Physical page frames are already inited!");

    root = early_kmalloc(sizeof(struct frame), KMALLOC_ZERO);
    //root->phys_addr = phys_addr;
    bitmap_memory = early_kmalloc(BITMAP_BIT_COUNT / 8, 0);
    bitmap_occupy(&root->bitmap, bitmap_memory, BITMAP_BIT_COUNT / 8);
    root->frames =
        early_kmalloc(BITMAP_BIT_COUNT * sizeof(struct frame *), KMALLOC_ZERO);

    for(int i = 0; i < BITMAP_BIT_COUNT; ++i) {
        struct frame *frame = early_kmalloc(sizeof(struct frame), KMALLOC_ZERO);

        //frame->phys_addr = phys_addr;
        bitmap_memory = early_kmalloc(BITMAP_BIT_COUNT / 8, 0);
        bitmap_occupy(&frame->bitmap, bitmap_memory, BITMAP_BIT_COUNT / 8);
        frame->up = root;
        (*root->frames)[i] = frame;
    }

    kprintf("OK\n");
}
