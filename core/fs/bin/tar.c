/**
 * @file    fs/bin/tar.c
 * @brief   TAR file parsing
 * @version 1.0
 * @date    26.09.2015.
 *
 * @author  Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright see file LICENSE
 */

#include <flos/types.h>
#include <tar.h>
#include <flos/kprintf.h>

unsigned int tar_hdr_filesz(const char *in) {
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;

    for(j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

void tar_foreach_file(addr_t addr, int (*f) (struct tar_hdr *)) {
    for(unsigned int i = 0;; i++) {
        struct tar_hdr *tar_hdr = (struct tar_hdr *)addr;

        if(tar_hdr->name[0] == '\0')
            break;

        if(f(tar_hdr))
            break;

        unsigned int filesz = tar_hdr_filesz(tar_hdr->size);

        if(filesz % 512)
            filesz = ((filesz / 512) + 1) * 512;
        addr += 512 + filesz;
    }
}
