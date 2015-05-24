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
    lib/bits.c \
    lib/ctype.c \
    lib/string.c \
    lib/vsprintf.c \
    mem/alloc.c \
    mem/frames.c \
    mem/tlsf.c \
	module/init.c \
    proc/interrupts.c \
    proc/sched.c

ifneq (,$(findstring TEST,$(features)))
    include tests/sources.mk
endif
