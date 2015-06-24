/**
 * @file    module/init.c
 * @brief   Module initialization
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>
#include <flos/list.h>
#include <flos/string.h>
#include <flos/kprintf.h>

#include "module.h"

/**
 * Finds module by its name
 */
struct module *module_find(const char *name) {
    struct module *m;

    list_for_each_entry(m, &modules, __modules) {
        if(!strcmp(name, m->name))
            return m;
    }

    return NULL;
}

/**
 * Resolve prerequisites
 */
int module_resolve_prereq(struct module *m) {
    if(m->state >= MODULE_PREREQ_SOLVED)
        return 0;

    for(struct module_prereq * p = m->parents; p != NULL && p->name != NULL;
        p++) {
        p->module = module_find(p->name);
    }

    for(struct module_prereq * p = m->required; p != NULL && p->name != NULL;
        p++) {
        if(!(p->module = module_find(p->name))) {
            kdebugf("Could not find required module '%s' by '%s'!\n", p->name,
                    m->name);
            return -1;
        }
    }

    m->state = MODULE_PREREQ_SOLVED;

    return 0;
}

/**
 * 	Resolves needed symbols
 */
int module_resolve_sym(struct module *m) {
    struct module_type_handler *h;

    if(m->type > STATIC_MODULE_TYPE) {
        list_for_each_entry(h, &handlers, __handlers) {
            if(h->type == m->type) {
                if(!h->resolve(m)) {
                    kdebugf("Could not resolve module '%s' symbols!", m->name);
                } else
                    m->state = MODULE_RESOLVED;

                break;
            }
        }
    } else
        m->state = MODULE_RESOLVED;

    if(m->state != MODULE_RESOLVED) {
        kdebugf("Unknown module '%s' type!\n", m->name);
        return -1;
    }

    return 0;
}

int module_init(struct module *m) {
    int err = 0;

    if(m->state == MODULE_STARTED)
        return 0;
    if(m->state > MODULE_STARTED)
        return -1;

    if(module_resolve_prereq(m)) {
        kdebugf("Could not resolve module '%s' prerequisites!\n", m->name);
        return -2;
    }

    if(m->required) {
        for(struct module_prereq * p = m->required;
            p != NULL && p->name != NULL; p++) {
            if(!p->module) {
                kdebugf("Module '%s' required by '%s' not found\n", p->name,
                        m->name);
                return -3;
            } else if(!module_init(p->module)) {
                kdebugf("Cannot init module '%s' required by '%s'\n", p->name,
                        m->name);
                return -4;
            }
        }
    }

    if(m->parents) {
        for(struct module_prereq * p = m->parents;
            p != NULL && p->name != NULL; p++) {
            if(p->module)
                module_init(p->module);
        }
    }

    if(module_resolve_sym(m)) {
        kdebugf("Could not resolve module '%s' symbols!\n", m->name);
        return -5;
    }

    if(m->init) {
        err = m->init();
    }

    if(!err)
        m->state = MODULE_STARTED;
    else
        kdebugf("Module '%s' returned error %d\n", err);

    return err;
}

int module_fini(struct module *m) {
    if(m->state != MODULE_STARTED)
        return -1;

    //TODO: notify others that module will finish

    if(m->exit) {
        m->exit();
    }

    m->state = MODULE_EXITED;

    return 0;
}
