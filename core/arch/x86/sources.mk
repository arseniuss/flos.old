#**
#* @file    arch/x86/sources.mk
#* @brief   List of x86 source files
#* @version 0.1
#* @date    28.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

defines += 32

includes += arch/x86/include

sources += \
	arch/x86/init/entry.S \
	arch/x86/init/kmain.c

