/**
 * @file    arch/x86/proc/callgate.S
 * @brief   Callgate assembler stub
 * @version 0.1
 * @date    10.10.2015.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

[section .text]

#define __PREPROCESSING__           1
#include <flos/arch/descriptors.h>

global callgate_entry
extern callgate_handler


/*          Stack
 *              old_ss
 *              old_esp
 *              old_cs
 *              old_eip
 *              params
 *
 */
callgate_entry:
    call callgate_handler

    retf
