#**
#* @file    debug/sources.mk
#* @brief   List of debug source files
#* @version 0.1
#* @date    28.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

sources +=  \
    debug/kprintf.c

ifneq (,$(findstring SERIALLOG,$(features)))
    sources += debug/log/serial.c
    DEFINES += KLOG=serial_puts
endif

ifneq (,$(findstring bochs,$(config)))
    DEFINES += CONFIG_BOCHS
endif
