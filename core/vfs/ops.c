/**
 * @file    vfs/ops.c
 * @brief   VFS operations
 * @version 1.0
 * @date    07.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/vfs.h>
#include <flos/kprintf.h>
#include <flos/types.h>
#include <flos/assert.h>
#include <flos/kernel.h>

#include "vfs.h"

struct fs_file *vfs_open_file(char *name, int flags, int mode) {
    kdebugf("vfs_open_file %s %b %b\n", name, flags, mode);

    return NULL;
}

struct fs_dir *vfs_open_dir(char *name) {
    kdebugf("vfs_open_dir %s\n", name);

    return NULL;
}

int vfs_close_file(struct fs_file *file) {
    kdebugf("vfs_close_file \n");

    return 1;
}


int vfs_mount_fs(char *path, struct fs_node *root_node) {
    kdebugf("vfs_mount_fs %s\n", path);


}

struct fs_node *vfs_alloc_fs_node(void) {
    struct fs_node *node = (struct fs_node *)kmalloc(sizeof(struct fs_node),
                                                     KMALLOC_ZERO);

    return node;
}
