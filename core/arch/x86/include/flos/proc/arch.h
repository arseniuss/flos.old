/**
 * @file    arch/x86/include/flos/proc/arch.h
 * @brief   Architecture dependent strcutures of process
 * @version 1.0
 * @date    24.05.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__PROC_ARCH_H__
#    define __flos_x86__PROC_ARCH_H__

#    include <flos/mem/paging.h>

struct process_arch {
    struct page_directory *page_directory;
};

#endif /* __flos_x86__PROC_ARCH_H__ */
