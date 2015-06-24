#!/bin/make -f

#**
#* @file    initrd.mk
#* @brief   Initialization ramdisk creating script
#* @version 0.1
#* @date    24.06.2015.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

include config.mk
include drivers/config.mk

modules = $(addprefix drivers/,$(addsuffix .ko,$(DYNAMIC_DRIVERS)))

.PHONY: initrd.tar

all: initrd.tar

initrd.tar: $(modules)
	@ /bin/echo -e "\t TAR initrd"
	@ if [ -f initrd.tar ]; then rm initrd.tar; fi;
	@ tar -cf initrd.tar $(modules)

clean:
	@ $(RM)  -v -rf initrd.tar
