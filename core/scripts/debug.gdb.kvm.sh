#!/usr/bin/env sh

#**
#* @file    debug.gdb.kvm.sh
#* @brief   Script to debug kernel with gdb in kvm (qemu)
#* @version 0.1
#* @date    13.12.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

if [ -f floppy.img ]; then
	if type "kvm" > /dev/null; then
		kvm -fda floppy.img -serial stdio
	fi
fi
