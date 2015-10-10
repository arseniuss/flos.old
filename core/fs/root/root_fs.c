/**
 * @file    fs/root/root_fs.c
 * @brief   ROOT file system
 * @version 1.0
 * @date    07.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/assert.h>
#include <flos/types.h>
#include <flos/vfs.h>
#include <flos/dev.h>
#include <flos/kernel.h>

#include "root_fs.h"

int root_fs_mount(struct fs_root *root, const char *args);
int root_fs_unmount(struct fs_root *root);

struct fs_node *root_fs_alloc_node(struct fs_root *);
void root_fs_destroy_node(struct fs_node *);
void read_node(struct fs_node *);
void write_node(struct fs_node *);
void update_node(struct fs_node *);
void delete_node(struct fs_node *);
void update_root(struct fs_root *);
void unmount_root(struct fs_root *);

int root_fs_create(struct fs_node *, struct fs_dir *, u32 perm);
int root_fs_release(struct fs_node *);
struct fs_node *root_fs_walk(struct fs_node *, char *, enum node_type);
int root_fs_setstat(struct fs_node *, struct fs_stat *);
struct fs_stat *root_fs_getstat(struct fs_node *);

struct fs root_fs = {
    .name = "root",
    .flags = FS_NO_MOUNTS,
    .usage = 1,                        // Always above 1

    .mount = &root_fs_mount,
    .unmount = &root_fs_unmount
};

struct fs_root_ops root_fs_root_ops = {
    .alloc_node = &root_fs_alloc_node,
    .destroy_node = &root_fs_destroy_node
};

struct fs_node_ops root_fs_node_ops = {
    .create = &root_fs_create,
    .release = &root_fs_release,
    .walk = &root_fs_walk,
    .setstat = &root_fs_setstat,
    .getstat = &root_fs_getstat
};

struct fs_file_ops root_fs_file_ops = {

};

int root_fs_mount(struct fs_root *root, const char *args) {
    if(!root)
        return 1;

    root->dev = DEVICE_ROOT;
    root->block_size = 0;
    root->max_file_size = 0;

    root->fs = &root_fs;

    return 0;
}

int root_fs_unmount(struct fs_root *root) {
    return 1;
}

static int root_fs_last_node_no = 0;
int root_fs_new_node_no(void) {
    return root_fs_last_node_no++;
}

// ROOT OPERATIONS {

struct fs_node *root_fs_alloc_node(struct fs_root *root) {
    struct fs_node *node = vfs_alloc_fs_node();

    node->no = root_fs_new_node_no();
    node->dev = DEVICE_ROOT;

    node->root = root;
    node->ops = &root_fs_node_ops;
    node->f_ops = &root_fs_file_ops;

    node->__private = kmalloc(sizeof(struct root_fs_private), KMALLOC_ZERO);

    return node;
}

void root_fs_destroy_node(struct fs_node *node) {
    kmfree(node);
}

// }

// NODE OPERATIONS {

int root_fs_create(struct fs_node *, struct fs_dir *, u32 perm) {

}

int root_fs_release(struct fs_node *) {

}

struct fs_node *root_fs_walk(struct fs_node *, char *, enum node_type) {

}

int root_fs_setstat(struct fs_node *, struct fs_stat *) {

}

struct fs_stat *root_fs_getstat(struct fs_node *) {

}

// }
