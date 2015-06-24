/**
 * @file    arch/x86/init/multiboot.c
 * @brief   x86 multiboot
 * @version 1.0
 * @date    24.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>
#include <multiboot.h>
#include <flos/kprintf.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

struct multiboot_info *multiboot_info = NULL;
u32 multiboot_magic = 0;

void dump_module_list() {
    if(CHECK_FLAG(multiboot_info->flags, 3)) {
        multiboot_module_t *mod;
        int i;

        kinfof("mods_count = %d, mods_addr = 0x%08x\n",
               multiboot_info->mods_count, multiboot_info->mods_addr);

        for(i = 0, mod = (multiboot_module_t *) multiboot_info->mods_addr;
            i < multiboot_info->mods_count; i++, mod++) {
            kinfof(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                   mod->mod_start, mod->mod_end, mod->cmdline);
        }
    }
}

void init_multiboot() {
    if(!multiboot_info || multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kwarningf("Multiboot structure not found! (magic: %x)",
                  multiboot_magic);
        return;
    }
#ifdef __HIGHER_HALF__
    multiboot_info->mmap_addr += KERNEL_VIRTUAL_BASE;
    multiboot_info->mods_addr += KERNEL_VIRTUAL_BASE;
    multiboot_info->cmdline += KERNEL_VIRTUAL_BASE;
    multiboot_info->drives_addr += KERNEL_VIRTUAL_BASE;
#endif

    kdebugf("Multiboot:\n");
    kdebugf("\tcmdline @ 0x%08x\n", multiboot_info->cmdline);
    kdebugf("\tmods @ 0x%08x\n", multiboot_info->mods_addr);
    kdebugf("\tdrives @ 0x%08x\n", multiboot_info->drives_addr);
    kdebugf("\tconf_table @ 0x%08x\n", multiboot_info->config_table);
    kdebugf("\tboot loader name @ 0x%08x\n", multiboot_info->boot_loader_name);
    kdebugf("\tapm_table @ 0x%08x\n", multiboot_info->apm_table);

    dump_module_list();
}
