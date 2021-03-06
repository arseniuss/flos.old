/**
 * @file    include/flos/fs/bin/tar.h
 * @brief   TAR file parsing
 * @version 1.0
 * @date    23.06.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#ifndef __flos__FS_BIN_TAR_H__
#    define __flos__FS_BIN_TAR_H__

#    include <flos/types.h>
#    include <tar.h>

void tar_foreach_file(addr_t addr, int (*f) (struct tar_hdr *));

#endif /* __flos__FS_BIN_TAR_H__ */
