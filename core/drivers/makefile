#!/usr/bin/env make -f

#**
#* @file    drivers/makefile
#* @brief   driver makefile
#* @version 0.1
#* @date    24.05.2015.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

includes = include
sources =
defines = MODULE

include ../config.mk
include ../common.mk

ifeq ($(arch),)
    $(error Architecture is not specified)
endif

include ../arch/$(arch)/config.mk
include config.mk

# Define CONFIG cause it is used in driver config.mk and sources.mk
CONFIG = $(addprefix CONFIG_,$(config))

ifneq ("$(driver)", "")
	ifneq ("$(wildcard $(addsuffix /config.mk,$(driver:%.ko=%)))", "")
		include $(addsuffix /config.mk,$(driver:%.ko=%))
	endif

	ifneq ("$(wildcard $(addsuffix /sources.mk,$(driver:%.ko=%)))", "")
		include $(addsuffix /sources.mk,$(driver:%.ko=%))
	endif
endif

config := $(shell echo $(config) | tr 'a-z' 'A-Z')
DEFINES += $(addsuffix =1,$(addprefix CONFIG_,$(config)))
defines += $(arch) $(platform) $(features)

# Temporary directory
TMP ?= $(CURDIR)/../tmp-$(arch)/drivers

# Reforming lists
INCLUDES    := $(addprefix -I../,$(includes))
DEFINES	    := $(addprefix -D__,$(addsuffix __=1,$(defines))) \
    $(addprefix -D,$(DEFINES)) \
    -DKERNEL_MAJOR=$(KERNEL_MAJOR) -DKERNEL_MINOR=$(KERNEL_MINOR) \
    -DKERNEL_PATCH=$(KERNEL_PATCH) -DKERNEL_NAME=\"$(KERNEL_NAME)\"

CFLAGS += $(INCLUDES) $(DEFINES)
CPPFLAGS += $(INCLUDES) $(DEFINES)
PPFLAGS += $(INCLUDES) $(DEFINES)

LDFLAGS += -Map $(TMP)/$(driver).map

CFLAGS += $(FLAGS)
CPPFLAGS += $(FLAGS)

sources := $(sources:drivers/%=%)
drivers := $(addsuffix .ko, $(DYNAMIC_DRIVERS))

objects	= $(addsuffix .o,$(sources))
objects	:= $(join $(dir $(objects)),$(notdir $(objects)))
objects	:= $(addprefix $(TMP)/,$(objects))
dump	= $(addsuffix .dump,$(objects))
deps	:= $(addsuffix .dep,$(objects))

sources := $(addprefix $(CURDIR)/,$(sources))

# Root of recursion
ifeq ("$(driver)", "")
all: $(drivers)

$(drivers):
	make driver=$@

else
all: $(driver)

$(driver): $(objects) $(CURDIR)/../arch/$(arch)/module.ld
	@ /bin/echo -e "\t LD $@"
	@ mkdir -p $(TMP)/arch/$(arch)
	@ $(PP) $(PPFLAGS) -P $(CURDIR)/../arch/$(arch)/module.ld > $(TMP)/arch/$(arch)/module.ld
	@ $(LD) $(LDFLAGS) -T $(TMP)/arch/$(arch)/module.ld -o $@ $(objects)
	@ $(DUMP) $(DUMPFLAGS) $@ > $(TMP)/$(notdir $@.dump)
	@ /bin/echo -e -n "$(OK)"

clear: clean
	@ rm -fv $(driver)
endif

$(TMP)/%.c.o: $(CURDIR)/%.c
	@ /bin/echo -e "\t C $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -E $< > $@.E
	@   $(CC) $(CFLAGS) -c -o $@ $<
	@ $(CC) $(CFLAGS) -M -MT $@  $< > $@.dep
	@ $(DUMP) $(DUMPFLAGS) $@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

$(TMP)/%.cpp.o: $(CURDIR)/%.cpp
	@ /bin/echo -e  "\t CPP $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(CPP) $(CPPFLAGS) -c -o $@ $<
	@ $(CPP) -M -MT $@ $(CPPFLAGS) $< > $@.dep
	@ $(CPP) $(CPPFLAGS) -E $< > $@.E
	@ $(DUMP) $(DUMPFLAGS) $(TMP)/$@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

$(TMP)/%.asm.o: $(CURDIR)/%.asm
	@ /bin/echo -e "\t ASM $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(ASM) $(ASMFLAGS) -c -o $@ $<
	@ $(DUMP) $(DUMPFLAGS) $@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

$(TMP)/%.S.o: $(CURDIR)/%.S
	@ /bin/echo -e "\t ASM $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(PP) $(PPFLAGS) $< > $@.asm
	@ $(PP) -M -MT $@ $(PPFLAGS) $< > $@.dep
	@ $(PP) $(PPFLAGS) -E $< > $@.E
	@ $(ASM) $(ASMFLAGS) -o $@ $@.asm
	@ $(DUMP) $(DUMPFLAGS) $@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

clean:
	@ rm -rvf $(TMP)

show-sources:
	@ echo $(sources)

show-objects:
	@ echo $(objects)

show-configs:
	@ echo $(config)

show-defines:
	@ echo $(defines)

show-CONFIG:
	@ echo $(CONFIG)

show-DEFINES:
	@ echo $(DEFINES)
