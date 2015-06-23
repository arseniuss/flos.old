/**
 * @file    include/flos/fs/bin/elf.h
 * @brief   ELF file parsing
 * @version 1.0
 * @date    23.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__FS_BIN_ELF_H__
#    define __flos__FS_BIN_ELF_H__

#    include <elf.h>
#    include <flos/defs.h>

__linkage int elf_relocate(void *addr, Elf32_Ehdr * h, char *name);

#endif /* __flos__FS_BIN_ELF_H__ */
