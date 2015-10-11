/**
 * @file    arch/x86/arch/init.h
 * @brief   x86 initialization functions
 * @version 1.0
 * @date    11.10.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__init_h__
#    define __flos_x86__init_h__

void multiboot_init(void);

int process_init(void);

void callgate_init(void);

void tss_init(void);

int timer_init(void);

void modules_init(void);

void multiboot_module_init(void);

void pool_init(void);

void paging_init(void);

void frames_init(void);

void mem_area_init(void);

int idt_init(void);

#endif
