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

enum ELF_GET_SECTION_BY {
    EGSB_IDX,
    EGSB_NAME
};

const char *elf_lookup_str(Elf32_Ehdr * h, u32 idx);
Elf32_Ehdr *elf_valid(addr_t addr);
__linkage int elf_relocate(void *addr, Elf32_Ehdr * h, char *name);
Elf32_Shdr *elf_get_section(Elf32_Ehdr * h, enum ELF_GET_SECTION_BY by, ...);

#endif /* __flos__FS_BIN_ELF_H__ */
