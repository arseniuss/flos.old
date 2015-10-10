/**
 * @file    vfs/dev.c
 * @brief   VFS devices
 * @version 1.0
 * @date    10.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/dev.h>
#include <flos/list.h>
#include <flos/string.h>

static LIST_HEAD(devices);

int register_device(struct dev *device) {

    list_add_tail(&device->__devices, &devices);

    return 0;
}

int unregister_device(const char *name) {
    struct dev *pos;

    list_for_each_entry(pos, devices, __devices) {
        if(!strncmp(name, pos->name, DEV_NAME_MAX)) {
            list_del(&pos->__devices);

            return 0;
        }
    }

    return -1;
}
