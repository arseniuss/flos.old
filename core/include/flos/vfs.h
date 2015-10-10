 /**
 * @file    include/flos/vfs.h
 * @brief   Virtual File System definitions
 * @version 1.0
 * @date    07.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__VFS_H__
#    define __flos__VFS_H__

#    include <flos/types.h>
#    include <flos/list.h>
#    include <flos/module.h>
#    include <flos/dev.h>

#    define X                               1
#    define W                               2
#    define R                               3
#    define RX                              4
#    define RW                              5
#    define RWX                             6

#    define PERM(u,g,o)                     (o | g << 3 | u << 6)

struct fs_root;
struct fs_node;
struct fs_file;
struct fs_dir;
struct fs;
struct fs_file_ops;

typedef u32 node_no_t;

enum node_type {
    NODE_UNKNOWN_TYPE = 0,
    NODE_FILE = 1,
    NODE_DIRECTORY = 2,
    NODE_PIPE = 3,
    NODE_SYMLINK = 4
};

enum node_flags {
    NODE_DIRTY = 1
};

struct fs_stat {
    dev_no_t dev;
    node_no_t node;

    u32 uid;
    u32 gid;

    u32 ctime;
    u32 mtime;
    u32 atime;
};

struct fs_node_ops {
    // Creates new node in directory
    int (*create) (struct fs_node *, struct fs_dir *, u32 perm);
    // Releases node (ready for writing into disk)
    int (*release) (struct fs_node *);
    // Seach for subnode with name
    struct fs_node *(*walk) (struct fs_node *, char *, enum node_type);
    // Set node statistics
    int (*setstat) (struct fs_node *, struct fs_stat *);
    // Get node statistics
    struct fs_stat *(*getstat) (struct fs_node *);
};

struct fs_node {
    node_no_t no;
    dev_no_t dev;
    __unsigned usage;

    u16 perm;
    u32 uid;
    u32 gid;

    enum node_type type;
    u16 flags;
    size_t size;
    size_t block_size;
    size_t blocks;

    u32 ctime;
    u32 mtime;
    u32 atime;

    struct fs_root *root;
    struct fs_node_ops *ops;
    struct fs_file_ops *f_ops;

    void *__private;
};

enum file_flags {
    FILE_IS_DIRTY = 1,
    FILE_IS_OPEN = 2
};

enum file_modes {
    FILE_MODE_READ = 1,
    FILE_MODE_WRITE = 2,
    FILE_MODE_APPEND = 4,
};

struct fs_file_ops {
    // Seek into file
    off_t(*seek) (struct fs_file *, off_t);
    // Reads data
    size_t(*read) (struct fs_file *, off_t, size_t, void *);
    // Write data
    size_t(*write) (struct fs_file *, off_t, size_t, void *);
    // Update file from disk
    void (*update) (struct fs_file *);
    // Release
    void (*release) (struct fs_file *);
    // Open file with mode
    int (*open) (struct fs_file *, u32);
    // Close file
    int (*close) (struct fs_file *);
};

struct fs_file {
    u32 flags;
    u32 mode;

    off_t off;
    size_t size;

    u32 perm;
    u32 uid;
    u32 gid;

    struct fs_file_ops *ops;

    struct list_head __root;
};

struct fs_dir {
    __unsigned usage;

    struct fs_node *node;

    struct fs_root *root;

    struct list_head children;  // children fs_dir
    struct list_head subdirs;
};

struct fs_root_ops {
    // Allocate new node from root
    struct fs_node *(*alloc_node) (struct fs_root *);
    // Deallocates node
    void (*destroy_node) (struct fs_node *);
    // Reads node with number node->no
    void (*read_node) (struct fs_node *);
    // Writes node into root
    void (*write_node) (struct fs_node *);
    // Update node (when reference count decreases)
    void (*update_node) (struct fs_node *);
    // Delete node from disk
    void (*delete_node) (struct fs_node *);
    // Update root (write into disk)
    void (*update_root) (struct fs_root *);
    //
    void (*unmount_root) (struct fs_root *);
};

struct fs_root {
    dev_no_t dev;
    u32 flags;                  // mount flags

    size_t block_size;
    size_t max_file_size;

    struct fs *fs;

    struct fs_root_ops *ops;

    struct list_head files;
    struct list_head dirty_nodes;
    struct list_head nodes;
    struct list_head __fs_roots;
};

#    define MAX_FS_NAME     32

enum fs_flags {
    FS_REQUIRES_DEV = 1,
    FS_NOT_USER = 2,
    FS_NO_DIRECTORIES = 4,
    // User cannot mount this filesystem
    FS_NO_MOUNTS = 8,
    FS_READONLY = 16
};

struct fs {
    char name[MAX_FS_NAME];
    u32 flags;
    __unsigned usage;

    struct module *owner;

    // Mount should do:
    //  - read root structure from device
    //  - verify filesystem magic
    //  - initialise root ops
    //  - allocate root node and root dir
    //  - set dirty to 1, if not readonly
    int (*mount) (struct fs_root *, const char *);
    int (*unmount) (struct fs_root *);

    struct list_head roots;
    struct list_head __filesystems;
};

int register_filesystem(struct fs *fs);
int unregister_filesystem(struct fs *fs);

struct fs_node *vfs_alloc_fs_node(void);

#endif
