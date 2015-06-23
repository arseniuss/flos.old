/**
 * @file    include/flos/fs/bin/sym.h
 * @brief   Kernel symbol table
 * @version 1.0
 * @date    23.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__FS_BIN_SYM_H__
#    define __flos__FS_BIN_SYM_H__

#    include <flos/defs.h>
#    include <flos/types.h>

/**
 * Export symbol to symbol table
 *
 * Creates linker time symbol into symbol table
 * @param  sym symbol
 */
#    define EXPORT_SYMBOL(sym)                                                 \
        extern typeof(sym) sym;                                                \
        static const char kstrtbl_##sym[] __used __section(".kstrtbl") = #sym; \
        static const struct ksymbol ksymtbl_##sym                              \
            __used __section(".ksymtbl_" #sym) = {(addr_t)&sym, kstrtbl_##sym};

/**
 * Kernel symbol
 */
struct ksymbol {
    addr_t addr;
    const char *name;
};

/**
 * Looks up kernel symbol
 * @param  name name of kernel symbol
 * @return      0 if not found
 */
__linkage addr_t sym_lookup(const char *name);

#endif /* __flos__FS_BIN_SYM_H__ */
