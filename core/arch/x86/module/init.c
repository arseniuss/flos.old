/**
 * @file    arch/x86/module/init.c
 * @brief   Module initialization
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/module.h>

extern char *__kernel_modules_start__;
extern char *__kernel_modules_end__;

struct module *kernel_modules_start =
    (struct module *)&__kernel_modules_start__;
struct module *kernel_modules_end = (struct module *)&__kernel_modules_end__;
