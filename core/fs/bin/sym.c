/**
 * @file    fs/bin/sym.c
 * @brief   Kernel symbols
 * @version 1.0
 * @date    24.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>
#include <flos/arch/mem.h>
#include <flos/fs/bin/sym.h>
#include <flos/string.h>
#include <flos/kprintf.h>

addr_t sym_lookup(const char *name) {
    if(!name)
        return NULL;

    struct ksymbol *sym;

    for(sym = (struct ksymbol *)kernel_symtbl_start;
        (u32) sym < (u32) kernel_symtbl_end; sym++) {
        if(!strcmp(sym->name, name)) {
            return sym->addr;
        }
    }

    kwarningf("Symbol %s not found!\n", name);

    return NULL;
}
