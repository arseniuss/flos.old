/**
 * @file    include/flos/mem/area.h
 * @brief   Virtual memory areas
 * @version 1.0
 * @date    03.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__MEM_AREA_H__
#    define __flos__MEM_AREA_H__

#    include <flos/types.h>
#    include <flos/module.h>
#    include <flos/list.h>

/** allow reading from area */
#    define MA_READ                        1

/** allow writing to area */
#    define MA_WRITE                       2

/** allow executing area */
#    define MA_EXEC                        4

/** area used by multiple processes */
#    define MA_SHARED                      8

/** memory maped IO device */
#    define MA_IO                          16

/** area grows up */
#    define MA_GROWSUP                     32

/** area grows down */
#    define MA_GROWSDOWN                   64

/** to do not swap or move area (for page directories & tables)*/
#    define MA_RESERVED                    128

/** copy-on-write */
#    define MA_COPYONWRITE                 256

struct memarea;

struct memarea_operations {
    int (*expand) (struct memarea * ma, size_t size);
    int (*shrink) (struct memarea * ma, size_t size);
    void *(*alloc) (struct memarea * ma, size_t size, u16 flags);
    void (*free) (struct memarea * ma, void *ptr, size_t size);
    int (*point) (struct memarea * ma, void *ptr, size_t size);
};

struct memarea {
    addr_t start;
    addr_t end;

    u32 flags;
    struct memarea_operations *ops;

    struct module *owner;
    struct list_head __owners;
    void *priv;
};

struct memarea_list {
    struct memarea *ma;
    struct list_head __mmap;
};

static inline void *memarea_priv(struct memarea *ma) {
    return ma->priv;
}

#endif /* __flos__MEM_AREA_H__ */
