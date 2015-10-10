#**
#* @file    sources.mk
#* @brief   List of source files
#* @version 0.1
#* @date    28.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

include arch/$(arch)/sources.mk
include debug/sources.mk
include drivers/sources.mk

sources +=  \
		fs/bin/elf.c \
		fs/bin/sym.c \
		fs/bin/tar.c \
		fs/init.c \
    lib/bits.c \
    lib/ctype.c \
    lib/string.c \
		lib/time.c \
    lib/vsprintf.c \
    mem/alloc.c \
    mem/frames.c \
    mem/tlsf.c \
		module/if.c \
		module/init.c \
    proc/interrupts.c \
    proc/sched.c \
		sys/chdir.c \
		sys/close.c \
		sys/create.c \
		sys/open.c \
		sys/read.c \
		sys/remove.c \
		sys/seek.c \
		sys/write.c \
		vfs/main.c \
		vfs/ops.c

ifneq (,$(findstring TEST,$(features)))
    include tests/sources.mk
endif
