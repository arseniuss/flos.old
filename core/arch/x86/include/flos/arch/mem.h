/**
 * @file    arch/x86/include/flos/mem/arch.h
 * @brief
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__MEM_ARCH_H__
#    define __flos_x86__MEM_ARCH_H__

#    include <flos/types.h>
#    include <flos/defs.h>

extern char __kernel_paging_start__;
extern char __kernel_paging_end__;
extern char __kernel_virt_start__;
extern char __kernel_virt_end__;
extern char __kernel_text_start__;
extern char __kernel_text_end__;
extern char __kernel_rodata_start__;
extern char __kernel_rodata_end__;
extern char __kernel_data_start__;
extern char __kernel_data_end__;
extern char __kernel_bss_start__;
extern char __kernel_bss_end__;

extern addr_t kernel_virt_start;
extern addr_t kernel_virt_end;
extern addr_t kernel_text_start;
extern addr_t kernel_text_end;
extern addr_t kernel_rodata_start;
extern addr_t kernel_rodata_end;
extern addr_t kernel_data_start;
extern addr_t kernel_data_end;
extern addr_t kernel_bss_start;
extern addr_t kernel_bss_end;

__linkage void init_memory_areas();

#endif
