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
#include <flos/string.h>
#include <flos/types.h>
#include <flos/kprintf.h>
#include <flos/assert.h>
#include <flos/list.h>

extern struct module **kernel_modules_start;
extern struct module **kernel_modules_end;

LIST_HEAD(modules);

struct module *module_find(const char *name) {
    for(struct module ** m = kernel_modules_start; m < kernel_modules_end; ++m) {
        if(!strcmp(name, (*m)->name))
            return *m;
    }

    return NULL;
}

void module_resolve(struct module *m) {
    if(m->state >= MODULE_PREREQ_SOLVED)
        return;

    for(struct module_prereq * p = m->required; p != NULL && p->name != NULL;
        ++p) {
        p->module = module_find(p->name);
    }

    for(struct module_prereq * p = m->parents; p != NULL && p->name != NULL;
        ++p) {
        p->module = module_find(p->name);
    }

    m->state = MODULE_PREREQ_SOLVED;
}

int module_init(struct module *m) {
    if(m->state == MODULE_STARTED)
        return 0;
    if(m->state > MODULE_STARTED)
        return -1;

    m->state = MODULE_STARTED;

    if(m->required) {
        for(struct module_prereq * p = m->required;
            p != NULL && p->name != NULL; ++p) {
            if(!p->module) {
                kdebugf("Module '%s' required by '%s' not found!\n", p->name,
                        m->name);
                return -1;
            } else
                module_init(p->module);
        }
    }

    if(m->parents) {
        for(struct module_prereq * p = m->parents; p != NULL && p->name != NULL;
            ++p) {
            module_init(p->module);
        }
    }

    if(m->init) {
        kdebugf("Initing module %s\n", m->name);

        int ret = m->init();

        return ret;
    }

    return 0;
}

void module_fini(struct module *m) {
    if(m->state != MODULE_STARTED)
        return;

    m->state = MODULE_EXITED;

    //TODO: notify that module exited

    if(m->exit) {
        m->exit();
    }
}

void init_modules() {
    int module_count = 0;
    int modules_failed = 0;
    int modules_inited = 0;

    kinfof("Initing kernel static modules ... ");
    kdebugf("\n");

    assert(kernel_modules_start != NULL,
           "Error: kernel module start position not found!");

    kdebugf("Kernel modules = {[%p .. %p]}\n",
            kernel_modules_start, kernel_modules_end);

    for(struct module ** m = kernel_modules_start; m < kernel_modules_end; ++m) {
        module_count++;
        list_add_tail(&(*m)->__modules, &modules);
        (*m)->state = MODULE_UNINITED;
    }

    kdebugf("Found %d modules\n", module_count);

    for(struct module ** m = kernel_modules_start; m < kernel_modules_end; ++m)
        module_resolve(*m);

    for(struct module ** m = kernel_modules_start; m < kernel_modules_end; ++m) {
        kdebugf("Module @ %p [name '%s', init @ %p, exit @ %p]\n",
                *m, (*m)->name, (*m)->init, (*m)->exit);

        if(module_init(*m)) {
            kerrorf("Can't init module %s!\n", (*m)->name);
            modules_failed++;
        } else
            modules_inited++;
    }

    kdebugf("Inited %d modules\n", modules_inited);
    kdebugf("Failed %d modules\n", modules_failed);


    kinfof("OK\n");
}
