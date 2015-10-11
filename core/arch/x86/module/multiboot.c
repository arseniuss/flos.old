/**
 * @file    arch/x86/module/multiboot.c
 * @brief   Multiboot module initialization
 * @version 1.0
 * @date    22.09.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <multiboot.h>
#include <tar.h>
#include <elf.h>
#include <flos/string.h>
#include <flos/kprintf.h>
#include <flos/fs/bin/tar.h>
#include <flos/fs/bin/elf.h>
#include <flos/module.h>
#include <flos/init.h>

#define HAS_FLAG(flags, bit)       ((flags) & (1 << (bit)))
#define strends(s1, s2)                                                        \
    (strlen(s1) >= strlen(s2) && !strcmp(s1 + strlen(s1) - strlen(s2), s2))

extern struct multiboot_info *multiboot_info;

void create_module(struct tar_hdr *hdr) {
    kdebugf("Creating module @ %p from %s\n", hdr, hdr->name);

    Elf32_Ehdr *ehdr = elf_valid(((addr_t) hdr) + 512);

    if(ehdr) {
        Elf32_Shdr *shdr = elf_get_section(ehdr, EGSB_NAME, ".kmodule");

        if(shdr) {
            const char *sectionname = elf_lookup_str(ehdr, shdr->sh_name);
            kdebugf("Module section \"%s\"\n", sectionname);
            kdebugf("Module structure @ %p\n", shdr->sh_offset);

            struct module *module = (struct module *)shdr->sh_offset;

            kdebugf("Module name %s\n", module->name);
        } else {
            kerrorf
                ("Wrong kernel object format: could not find .kmodule section!\n");
        }
    } else {
        kerrorf("Invalid kernel object %s\n", hdr->name);
    }
}

void create_file(struct tar_hdr *hdr) {
    kdebugf("Creating file /initrd/%s\n", hdr->name);
}

int dump_header(struct tar_hdr *hdr) {
    if(strends(hdr->name, ".ko")) {
        kdebugf("File %s ends with \".ko\"\n", hdr->name);

        create_module(hdr);
    } else {
        create_file(hdr);
    }

    return 0;
}

int multiboot_module_init(void) {
    kinfof("Initing multiboot modules ...\n");

    if(HAS_FLAG(multiboot_info->flags, 3)) {
        multiboot_module_t *mod;
        int i;
        struct tar_hdr *tar_hdr;

        for(i = 0, mod = (multiboot_module_t *) multiboot_info->mods_addr;
            i < multiboot_info->mods_count; i++, mod++) {
            tar_hdr = (struct tar_hdr *)(mod->mod_start + 0xf0000000);

            kdebugf("TAR MAGIC: \"%s\"\n", tar_hdr->magic);

            if(!strncmp(tar_hdr->magic, TMAGIC, TMAGLEN - 1)) {
                kdebugf("Module is tar archive.\n");

                tar_foreach_file((addr_t) tar_hdr, &dump_header);
            } else {
                kerrorf("Module is not tar archive!\n");
            }
        }
    }

    kinfof("OK\n");

    return 0;
}

KINIT(multiboot_module_init, "multiboot", "modules");
