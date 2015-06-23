/**
 * @file    include/flos/debug/fs/bin/elf.h
 * @brief   ELF file debugging
 * @version 1.0
 * @date    23.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_debug__FS_BIN_ELF_H__
#    define __flos_debug__FS_BIN_ELF_H__

#    ifdef __DEBUG__
#        include <elf.h>

#        define ELF_TYPE(x)                                                     \
            ({char * ret;                                                      \
                switch(x) {                                                    \
                case ET_NONE: ret = "none"; break;                             \
                case ET_REL: ret = "rel"; break;                               \
                case ET_EXEC: ret = "exec"; break;                             \
                case ET_DYN: ret = "dyn"; break;                               \
                case ET_CORE: ret = "core"; break;                             \
                case ET_LOPROC: ret = "spec"; break;                           \
                case ET_HIPROC: ret = "spec"; break;                           \
                default: ret = "unknown"; break;                               \
            };                                                                 \
            ret;})

#        define ELF_MACHINE(x)                                                  \
            ({char * ret;                                                      \
                switch(x) {                                                    \
                case EM_NONE: ret = "none"; break;                             \
                case EM_M32: ret = "AT&T WE 32100"; break;                     \
                case EM_SPARC: ret = "SUN SPARC"; break;                       \
                case EM_386: ret = "Intel 80386"; break;                       \
                case EM_486: ret = "Intel 80486"; break;                       \
                case EM_860: ret = "Intel 80860"; break;                       \
                case EM_960: ret = "Intel 80960"; break;                       \
                case EM_IA_64: ret = "Intel Merced"; break;                    \
                default: ret = "unknown"; break;                               \
                };                                                             \
            ret;})

#        define ELF_FLAGS(x)            ""  //ELF don't have flags for intel!

#        define ELF_SYM_BIND(x)                                                 \
            ({char * ret;                                                      \
                switch(x) {                                                    \
                case STB_LOCAL: ret = "local"; break;                          \
                case STB_GLOBAL: ret = "global"; break;                        \
                case STB_WEAK: ret = "weak";  break;                           \
                default: ret = "unknown"; break;                               \
                };                                                             \
            ret;})

#        define ELF_SYM_TYPE(x)                                                 \
            ({char * ret;                                                      \
                switch(x) {                                                    \
                case STT_NOTYPE: ret = "no type"; break;                       \
                case STT_OBJECT: ret = "object"; break;                        \
                case STT_FUNC: ret = "function";  break;                       \
                case STT_SECTION: ret = "section";  break;                     \
                case STT_FILE: ret = "file";  break;                           \
                default: ret = "unknown"; break;                               \
                };                                                             \
            ret;})

#        define ELF_DUMP_HEADER(x, name, sz)                                    \
            ({Elf32_Ehdr *h = (x);                                             \
                DEBUG("========== ELF header for %s (size %x) ==========\n",   \
                    name, sz);                                                 \
                DEBUG("\ttype: %s\n", ELF_TYPE(h->e_type));                    \
                DEBUG("\tmachine: %s\n", ELF_MACHINE(h->e_machine));           \
                DEBUG("\tversion: %d\n", h->e_version);                        \
                DEBUG("\tentry @ 0x%08x\n", h->e_entry);                       \
                DEBUG("\tprogram header @ 0x%08x\n", h->e_phoff);              \
                DEBUG("\tsection headers @ 0x%08x count %d\n",                 \
                    elf_header->e_shoff, elf_header->e_shnum);                 \
                DEBUG("\tflags 0b%b\n", h->e_flags);                           \
                Elf32_Shdr *elf_section =(Elf32_Shdr *)(h->e_shoff + (u32)h);  \
                const char *elf_strtbl = (const char *)                        \
                    (elf_section[h->e_shstrndx].sh_offset + (u32) h);          \
                    DEBUG("\tstring table section @0x%08x\n", elf_strtbl);     \
                for (int i = 0; i < h->e_shnum; i++) {                         \
                    DEBUG("\tsection %d. @ 0x%08x\n", i + 1, elf_section);     \
                    DEBUG("\t\tname @ 0x%08x (%s)\n", elf_section->sh_name,    \
                        &elf_strtbl[elf_section->sh_name]);                    \
                    DEBUG("\t\tstart @ 0x%08x\n", elf_section->sh_offset);     \
                    DEBUG("\t\tsize @ 0x%08x\n", elf_section->sh_size);        \
                    elf_section++;                                             \
                }                                                              \
                DEBUG("========== ========== ========== ==========\n");        \
            })

#    endif
#endif /* __flos_debug__FS_BIN_ELF_H__ */
