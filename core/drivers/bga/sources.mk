#**
#* @file    drivers/bga/sources.mk
#* @brief   List of Bochs Graphics Adapter driver source files
#* @version 0.1
#* @date    14.06.2015.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

includes += \
	drivers/bga/arch/x86/include

sources += \
	drivers/bga/bga.c \
	drivers/bga/main.c
