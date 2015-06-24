/**
 * @file    fs/bin/elf.c
 * @brief   ELF file parsing
 * @version 1.0
 * @date    24.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <elf.h>
#include <flos/assert.h>
#include <flos/types.h>
#include <flos/vaargs.h>
#include <flos/string.h>
#include <flos/fs/bin/sym.h>

#include <flos/debug/fs/bin/elf.h>

enum ELF_GET_SECTION_BY {
    EGSB_IDX,
    EGSB_NAME
};

Elf32_Shdr *elf_get_section(Elf32_Ehdr * h, enum ELF_GET_SECTION_BY by, ...) {
    Elf32_Shdr *elf_section = (Elf32_Shdr *) (h->e_shoff + (u32) h);
    const char *elf_strtbl =
        (const char *)(elf_section[h->e_shstrndx].sh_offset + (u32) h);

    va_list args;
    va_start(args, by);

    switch (by) {
        case EGSB_IDX:
        {
            int idx = va_arg(args, int);

            va_end(args);

            return &elf_section[idx];
        }
            break;
        case EGSB_NAME:
        {
            const char *name = va_arg(args, const char *);

            for(int i = 0; i < h->e_shnum; i++) {
                if(!strcmp(name, &elf_strtbl[elf_section->sh_name])) {
                    if(elf_section->sh_offset < (u32) h) {
                        elf_section->sh_offset += (u32) h;
                    }

                    va_end(args);
                    return elf_section;
                }

                elf_section++;
            }
        }
            break;
    }

    return NULL;
}

const char *elf_lookup_str(Elf32_Ehdr * h, u32 idx) {
    Elf32_Shdr *strtbl = elf_get_section(h, EGSB_NAME, ".strtab");
    assert(strtbl);

    const char *str = (const char *)(strtbl->sh_offset + idx);

    return str;
}

enum ELF_LOOKUP_SYMBOL_BY {
    ELSB_REL,
    ELSB_NAME,
    ELSB_NAME_LENGTH,
};

addr_t elf_lookup_symbol(Elf32_Ehdr * h, enum ELF_LOOKUP_SYMBOL_BY by, ...) {
    Elf32_Shdr *symtbl = elf_get_section(h, EGSB_NAME, ".symtab");
    assert(symtbl);

    va_list args;
    va_start(args, by);

    Elf32_Sym *sym;
    Elf32_Rel *r = NULL;

    switch (by) {
        case ELSB_REL:
            r = va_arg(args, Elf32_Rel *);
            sym = (Elf32_Sym *) (symtbl->sh_offset +
                                 ELF32_R_SYM(r->r_info) * sizeof(Elf32_Sym));
            break;
        case ELSB_NAME:
        case ELSB_NAME_LENGTH:
            sym = (Elf32_Sym *) (symtbl->sh_offset);

            const char *name = va_arg(args, const char *);

            for(; (u32) sym < symtbl->sh_offset + symtbl->sh_size; sym++) {
                const char *symname = elf_lookup_str(h, sym->st_name);

                if(by == ELSB_NAME_LENGTH) {
                    int l = va_arg(args, int);

                    if(!strncmp(symname, name, l)) {
                        goto out;
                    }

                } else if(!strcmp(symname, name)) {
                    goto out;
                }
            }

            return NULL;
        default:
            return NULL;
    }

  out:
    //DEBUG("symbol %s\n", elf_lookup_str(h, sym->st_name));

    switch (ELF32_ST_TYPE(sym->st_info)) {
        case STT_SECTION:
        {
            Elf32_Shdr *s = elf_get_section(h, EGSB_IDX, sym->st_shndx);
            assert(s);

            return s->sh_offset;
        }
        case STT_NOTYPE:
        {
            addr_t addr = sym_lookup(elf_lookup_str(h, sym->st_name));
            assert(addr);

            return addr;
        }
        case STT_FUNC:
        {
            addr_t addr = 0;

            if(sym->st_shndx) {
                Elf32_Shdr *s = elf_get_section(h, EGSB_IDX, sym->st_shndx);

                addr += s->sh_offset;
            }

            addr += sym->st_value;

            return addr;
        }
    }

#ifdef __DEBUG__
    kdebugf("symbol not found ");
    if(sym->st_name)
        kdebugf("name: %s ", elf_lookup_str(h, sym->st_name) ? : "");
    kdebugf("type: %s ", ELF_SYM_TYPE(ELF32_ST_TYPE(sym->st_info)));
    kdebugf("value: %x ", sym->st_value);
    kdebugf("section: %x ", sym->st_shndx);
    kdebugf("\n");
#endif

    return NULL;
}

int elf_rel_symbol(u32 * base, Elf32_Ehdr * h, void *r) {
    Elf32_Rel *rel = r;
    u32 *place = (u32 *) ((u32) base + rel->r_offset);
    addr_t addr;

    if(!(addr = elf_lookup_symbol(h, ELSB_REL, r))) {
        return -1;
    }

    switch (ELF32_R_TYPE(rel->r_info)) {
        case R_386_32:                /* S + A */
            *place += addr;
            break;
        case R_386_PC32:              /* S + A -P */
            *place += addr - (u32) place;
            break;
        default:
            kwarningf("Unknown symbol type!");
            return -1;
    }

    return 0;
}

int elf_relocate(void *addr, Elf32_Ehdr * h, char *name) {
    Elf32_Shdr *rel_section = elf_get_section(h, EGSB_NAME, name);

    if(!rel_section) {
        return -1;
    }

    Elf32_Rel *rel;

    for(rel = (Elf32_Rel *) rel_section->sh_offset;
        (u32) rel < rel_section->sh_offset + rel_section->sh_size; rel++) {

        int ret;

        if((ret = elf_rel_symbol(addr, h, rel))) {
            return ret;
        }
    }

    return 0;
}
