/**
 * @file    module/if.c
 * @brief   Module interface
 * @version 1.0
 * @date    24.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>

#include "module.h"

LIST_HEAD(modules);
LIST_HEAD(handlers);

int register_module(struct module *m) {
    // TODO: check if m is filled

    m->state = MODULE_UNINITED;

    list_add_tail(&m->__modules, &modules);

    return 0;
}

void unregister_module(struct module *m) {

}

int register_module_type_handler(struct module_type_handler *h) {
    //TODO: check if h is filled

    list_add_tail(&h->__handlers, &handlers);

    return 0;
}

void unregister_module_type_handler(struct module_type_handler *h) {

}
