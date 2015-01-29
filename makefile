#!/usr/bin/env make -f

#**
#* @file    Makefile
#* @brief   Project makefile
#* @version 0.1
#* @date    29.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

x86-rules = i386 i486 i586 i686

.PHONY: core

x86: | tools-i686 configure-i686 core
i386: | tools-i386 configure-i386 core
i486: | tools-i486 configure-i486 core
i586: | tools-i586 configure-i586 core
i686: | tools-i686 configure-i686 core

tools-%:
	cd gcc-cc; export ARCHES=$(subst tools-,,$@); ./doit

$(addprefix configure-,$(x86-rules)):
	. gcc-cc/toolvers && \
	./configure \
		--arch=x86 \
		--cc=gcc-cc/$(subst configure-,,$@)-elf-$$GCCVER-`uname`-`uname -m` \
		--ccp=$(subst configure-,,$@)-elf- \
		--force
		
$(addprefix configure-debug-,$(x86-rules)):
	. gcc-cc/toolvers && \
	./configure \
		--arch=x86 \
		--cc=gcc-cc/$(subst configure-debug-,,$@)-elf-$$GCCVER-`uname`-`uname -m` \
		--ccp=$(subst configure-debug-,,$@)-elf- \
		--config=bochs \
		--debug \
		--features=SERIALLOG \
		--force
		
core:
	@ make -C core

clean:
	cd gcc-cc; ./cleanit
	rm -vf config.mk
	rm -vf core/config.mk
	make -C core clean
