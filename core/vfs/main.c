/**
 * @file    vfs/main.c
 * @brief   VFS main
 * @version 1.0
 * @date    07.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/assert.h>
#include <flos/list.h>
#include <flos/kprintf.h>
#include <flos/string.h>
#include <flos/vfs.h>

#include "vfs.h"

LIST_HEAD(filesystems);

int register_filesystem(struct fs *fs) {
    assert(fs);

    kdebugf("register_filesystem %s\n", fs->name);

    if(strlen(fs->name) < 1)
        return 1;

    INIT_LIST_HEAD(&fs->roots);
    INIT_LIST_HEAD(&fs->__filesystems);

    list_add_tail(&fs->__filesystems, &filesystems);

    return 0;
}

int unregister_filesystem(struct fs *fs) {
    assert(fs);

    kdebugf("unregister_filesystem %s\n", fs->name);

    // TODO

    return 1;
}
