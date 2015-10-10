/**
 * @file    include/flos/dev.h
 * @brief   Device definitions
 * @version 1.0
 * @date    07.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__DEV_H__
#    define __flos__DEV_H__

#    include <flos/types.h>
#    include <flos/list.h>

#    define DEVICE_ROOT         0

#    define DEV_NAME_MAX        32

typedef u16 dev_no_t;

struct dev {
    char name[DEV_NAME_MAX];

    struct dev *parent;
    struct list_head __devices;
    void *__private;
};

int register_device(struct dev *device);
int unregister_device(const char *name);

#endif
