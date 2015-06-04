/**
 * @file    arch/x86/include/flos/mem/paging.h
 * @brief   Paging
 * @version 1.0
 * @date    08.12.2014.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos_x86__PAGING_H__
#    define __flos_x86__PAGING_H__

#    include <flos/defs.h>
#    include <flos/types.h>

/** Page directory of page table @{ */

/** Page directory entry is present */
#    define PDE_PRESENT                     0x001
/** Page directory entry readable */
#    define PDE_RW                          0x002
/** Page directory entry is accessable by user */
#    define PDE_USER                        0x004
/** Page directory entry is write-through */
#    define PDE_WT                          0x008
/** Page directory entry is cache disabled */
#    define PDE_CD                          0x010
/** Page directory entry has been accessed */
#    define PDE_ACCESSED                    0x020
/** Page directory entry to 4MB page is dirty */
#    define PDE4_DIRTY                      0x040
/** Page directory entry represents 4MB page */
#    define PDE_SIZE                        0x080
/** Page directory entry is global */
#    define PDE_GLOBAL                      0x100

/** @} */

/** Page table @{ */

/** Page table entry is present */
#    define PTE_PRESENT                     0x001
/** Page table entry is readable */
#    define PTE_RW                          0x002
/** Page table entry is accessable by user */
#    define PTE_USER                        0x004
/** Page table entry used write-through */
#    define PTE_WT                          0x008
/** Page table entry has cache disabled */
#    define PTE_CD                          0x010
/** Page table entry has been accessed */
#    define PTE_ACCESSED                    0x020
/** Page directory entry has been written to */
#    define PTE_DIRTY                       0x040
/** Page Attribute table */
#    define PTE_PAT                         0x080
/** Page table entry is global */
#    define PTE_GLOBAL                      0x100

/** @} */

#    define PAGE_FAULT_INTERRUPT            0x0E

typedef u32 pte_t;
typedef u32 pde_t;

/** Page table structure */
struct page_table {
    pte_t entry[1024] __align(4096);
} __packed;

/** Page directory structure */
struct page_directory {
    pde_t entry[1024] __align(4096);
    struct page_table *table[1024];
} __packed;

__linkage void init_paging();

#endif /* __flos_x86__PAGING_H__ */
