/**
 * @file    include/flos/module.h
 * @brief   Modules
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__MODULE_H__
#    define __flos__MODULE_H__

#    include <flos/defs.h>
#    include <flos/list.h>

/** Module states */
enum module_state {
    MODULE_UNINITED,            ///< Uninited module
    MODULE_PREREQ_SOLVED,       ///< Prerequisites solved
    MODULE_RESOLVED,            ///< Module symbols resolved
    MODULE_STARTED,             ///< Module init function run
    MODULE_EXITED               ///< Module exit function run
};

/** Module types */
enum module_type {
    STATIC_MODULE_TYPE = 0,     ///< Module is part of kernel
    ELF_MODULE_TYPE                    ///< Dynamicly loaded ELF object
};

/** A prerequisite of a module */
struct module_prereq {
    const char *name;
    struct module *module;
};

struct module {
    const char *name;           ///< module name
    const char *authors;        ///< module authors
    const char *description;    ///< module description

    enum module_type type;      ///< module type
    enum module_state state;    ///< module state

    /**
     * @short required modules
     * NULL or NULL-terminated list of prereq structures
     */
    struct module_prereq *required;

    /**
     * @short parent modules (not required)
     * NULL or NULL-terminated list of prereq structures
     */
    struct module_prereq *parents;

    int (*init) (void);         ///< pointer to module initialisation function
    void (*exit) (void);        ///< pointer to module finialisation function

    struct list_head memmap;
    struct list_head __modules; ///< node for list #kmodules

    void *specific;             ///< pointer to module data
};

struct module_type_handler {
    const char *name;           ///< module type handler name

    enum module_type type;      ///< module type

    int (*resolve) (struct module *);
    void (*finish) (struct module *);

    struct list_head __handlers;
};

#    define MODULE_INIT(fn)                                                    \
        int (*fn##_ctor)() __section(".module.ctor") = fn;

#    define MODULE_EXIT(fn)                                                    \
        void (*fn##_dtor)() __section(".module.dtor") = fn;

#    define INTERNAL_MODULE(mod)                                               \
        static struct module *UNIQUE(mod) __section(".kmodules") __used = &mod;

#    define EXTERNAL_MODULE(mod)                                               \
        static struct module *UNIQUE(mod) __section(".kmodule")__used = &mod;

int register_module(struct module *m);
void unregister_module(struct module *m);
int register_module_type_handler(struct module_type_handler *h);
void unregister_module_type_handler(struct module_type_handler *h);
struct module *module_find(const char *name);
int module_init(struct module *m);

#endif /* __flos__MODULE_H__ */
