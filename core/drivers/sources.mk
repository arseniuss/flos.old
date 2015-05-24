#**
#* @file    drivers/sources.mk
#* @brief   List of driver source files
#* @version 0.1
#* @date    28.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

include drivers/config.mk

ifndef STATIC_DRIVERS
	$(error "No static drivers defined!")
endif

driver_sources = $(addsuffix /sources.mk, $(addprefix drivers/,$(STATIC_DRIVERS)))

include $(driver_sources)
