/**
 * @file    init/kmain.c
 * @brief   Kernel initialization
 * @version 0.1
 * @date    11.10.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/init.h>
#include <flos/kprintf.h>
#include <flos/assert.h>
#include <flos/types.h>
#include <flos/string.h>

extern char __kernel_kinittbl_start__;
extern char __kernel_kinittbl_end__;

struct kinit *kinit = (struct kinit *)&__kernel_kinittbl_start__;

static struct kinit *kernel_find(const char *name) {
    struct kinit *p;
    char sname[100] = "\0";

    sprintf(sname, "%s_init", name);

    for(p = kinit; ((addr_t) p) < (addr_t) & __kernel_kinittbl_end__; p++) {
        if(!strncmp(p->name, sname, strlen(sname)))
            return p;
    }

    return NULL;
}

int kernel_initf(struct kinit *i) {
    char **p = i->deps->names;

    p++;
    while(*p != NULL) {
        struct kinit *dep = kernel_find(*p);

        if(!dep || dep == i) {
            kerrorf("Cannot resolve \"%s\"\n", *p);
            return -1;
        }

        if(!dep->inited && kernel_initf(dep)) {
            kerrorf("Cannot init \"%s\"\n", *p);
            return -2;
        }

        p++;
    }

    if(!i->kinit) {
        kerrorf("No init func for \"%s\"\n", i->name);
        return -3;
    }

    kinfof("Executing %s() ... \n", i->name);

    int ret = i->kinit();

    if(!ret)
        kinfof("%s() returned SUCCESS result!\n", i->name);
    else
        kerrorf("%s() returned FAILED result!\n", i->name);

    i->inited = TRUE;

    return ret;
}

int kernel_init(void) {
    assert(kinit != NULL);

    kdebugf("Kernel init funcs ={[%p .. %p]}\n",
            &__kernel_kinittbl_start__, &__kernel_kinittbl_end__);

    kinfof("Initing kernel ...\n");

    addr_t p = (addr_t) & __kernel_kinittbl_start__;
    for(; p < (addr_t) & __kernel_kinittbl_end__; p += sizeof(struct kinit)) {
        struct kinit *i = (struct kinit *)p;

        if(!i->inited && kernel_initf(i)) {
            return -1;
        }
    }

    kinfof("Kernel inited!\n");

    return 0;
}
