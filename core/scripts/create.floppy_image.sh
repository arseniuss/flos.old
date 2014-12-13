#!/usr/bin/env sh

#**
#* @file    create.floppy_image.sh
#* @brief   Script which create floppy image with GRUB
#* @version 0.1
#* @date    23.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

# Exit if any error
#set -e

if [ ! -f floppy.img ]; then
    # Getting floppy image file with GRUB installed
    wget -nc http://cstein.kings.cam.ac.uk/~chris/bootgrub.gz

    # Unzip into floppy image file
    gzip -d < bootgrub.gz | dd of=floppy.img
fi

# If image is already mounted then unmount it
mountpoint mnt/floppy 2>&1 > /dev/null
if [ $? -eq 0 ]; then
    sudo umount mnt/floppy
fi
mkdir -p mnt/floppy
sudo mount -o loop,umask=000 floppy.img mnt/floppy
echo "title flos operating system
        root (fd0)
        kernel /kernel" > mnt/floppy/boot/grub/menu.lst
rm -f mnt/floppy/kernel
cp kernel.x86.elf mnt/floppy/kernel
sudo umount mnt/floppy
