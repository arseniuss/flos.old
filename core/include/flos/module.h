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
    MODULE_PREREQ_SOLVED,       ///< Inited module
    MODULE_STARTED,             ///< Module init function run
    MODULE_EXITED               ///< Module exit function run
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
};

/** List of kernel modules */
extern struct list_head modules;

#    define MODULE_INIT(fn)                                                    \
        int (*fn##_ctor)() __section(".module.ctor") = fn;

#    define MODULE_EXIT(fn)                                                    \
        void (*fn##_dtor)() __section(".module.dtor") = fn;

#    define INTERNAL_MODULE(mod)                                               \
        static struct module *UNIQUE(mod) __section(".kmodules") __used = &mod;

void init_modules();

#endif /* __flos__MODULE_H__ */
