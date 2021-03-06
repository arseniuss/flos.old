#**
#* @file    arch/x86/sources.mk
#* @brief   List of x86 source files
#* @version 0.1
#* @date    28.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

sources += \
	arch/x86/init/entry.S \
	arch/x86/init/kmain.c \
	arch/x86/init/multiboot.c \
	arch/x86/mem/alloc.c \
	arch/x86/mem/area.c \
	arch/x86/mem/frames.c \
	arch/x86/mem/paging.c \
	arch/x86/mem/pool.c \
	arch/x86/module/init.c \
	arch/x86/module/multiboot.c \
	arch/x86/proc/callgate.S \
	arch/x86/proc/idt.c \
	arch/x86/proc/idt.S \
	arch/x86/proc/interrupt.c \
	arch/x86/proc/process.c \
	arch/x86/proc/syscall.c \
	arch/x86/proc/timer.c \
	arch/x86/proc/tss.c
