/**
 * @file    include/flos/bitmap.h
 * @brief   Bitmap structure
 * @version 1.0
 * @date    07.02.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__BITMAP_H__
#    define __flos__BITMAP_H__

#    include <flos/types.h>
#    include <flos/kernel.h>
#    include <flos/string.h>
#    include <flos/defs.h>

#    ifndef _BITMAP_FIELD_BITS
#        define _BITMAP_FIELD_BITS          32
#    endif

#    define _BITMAP_FIELD_TYPE              CONCAT(u, _BITMAP_FIELD_BITS)
#    define _BITMAP_FIELD_MAX               CONCAT(_BITMAP_FIELD_TYPE, _max)

#    define _BITMAP_INDEX(entry)            ((entry) / _BITMAP_FIELD_BITS)
#    define _BITMAP_OFFSET(entry)           ((entry) % _BITMAP_FIELD_BITS)

struct bitmap {
    size_t size;                ///< Size in bytes of map
    size_t bits;                ///< Used bit count in bitmap
    //TODO: lock
    void *map;
};

static inline void bitmap_init(struct bitmap *bm) {
    bm->size = 0;
    bm->map = NULL;
}

static inline int bitmap_alloc(struct bitmap *bm, int bits) {
    size_t size = bits / 8 + (bits % 8 ? 1 : 0);
    void *map;

    if((map = kmalloc(size, KMALLOC_ZERO)) == NULL) {
        return 1;
    }

    bm->bits = bits;
    bm->size = size;
    bm->map = map;

    return 0;
}

/**
 * Set bitmap to occupy specific memory location
 * @param bm   pointer to bitmap structure
 * @param ptr  pointer to memory location
 * @param size size of memory location
 */
static inline void bitmap_occupy(struct bitmap *bm, void *ptr, size_t size) {
    bm->map = ptr;
    bm->bits = size * 8;
    bm->size = size;

    memset(ptr, 0, size);
}

static inline void bitmap_zero(struct bitmap *bm) {
    memset(bm->map, 0, bm->size);
}

static inline void bitmap_set(struct bitmap *bm, unsigned entry) {
    int idx = _BITMAP_INDEX(entry);
    int off = _BITMAP_OFFSET(entry);

    ((_BITMAP_FIELD_TYPE *) bm->map)[idx] |= (0x1 << off);
}

static inline void bitmap_free(struct bitmap *bm, int entry) {
    int idx = _BITMAP_INDEX(entry);
    int off = _BITMAP_OFFSET(entry);

    ((_BITMAP_FIELD_TYPE *) bm->map)[idx] &= ~(0x1 << off);
}

static inline bool bitmap_test(struct bitmap *bm, int entry) {
    int idx = _BITMAP_INDEX(entry);
    int off = _BITMAP_OFFSET(entry);

    return (bool) (((_BITMAP_FIELD_TYPE *) bm->map)[idx] & (0x1 << off));
}

static inline int bitmap_first_free(struct bitmap *bm) {
    _BITMAP_FIELD_TYPE part;

    for(int i = 0; i < bm->size; ++i) {
        if(((_BITMAP_FIELD_TYPE *) bm->map)[i] != _BITMAP_FIELD_MAX) {
            part = ((_BITMAP_FIELD_TYPE *) bm->map)[i];

            for(int j = 0; j < _BITMAP_FIELD_BITS; ++j) {
                if(!(part & (0x1 << j)))
                    return i * _BITMAP_FIELD_BITS + j;
            }
        }
    }

    return -1;
}

#endif /* __flos__BITMAP_H__ */
