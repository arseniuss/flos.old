#!/usr/bin/env make -f

#**
#* @file    Makefile
#* @brief   Core makefile
#* @version 0.1
#* @date    27.11.2014.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

#*
#* TODO: Target depends on makefiles and source lists -- find new make program
#*

includes = include
sources =
defines = KERNEL

ifeq ("$(wildcard ./config.mk)","")
    $(error Configuration file config.mk do not exist)
endif

include common.mk
include config.mk

ifeq ($(arch),)
    $(error Architecture is not specified)
endif

include arch/$(arch)/config.mk

CONFIG = $(addprefix CONFIG_,$(config))

include sources.mk

ifneq ($(notdir $(ASM)),nasm)
    $(error $(notdir $(ASM)) assembler is not supported. Use nasm assembler)
endif

config := $(shell echo $(config) | tr 'a-z' 'A-Z')
DEFINES += $(addsuffix =1,$(addprefix CONFIG_,$(config)))
defines += $(arch) $(platform) $(features)

# Temporary directory
TMP = $(CURDIR)/tmp-$(arch)
# Target binary
TARGET = $(CURDIR)/kernel.$(arch).elf

# Reforming lists
INCLUDES    := $(addprefix -I,$(includes))
DEFINES	    := $(addprefix -D__,$(addsuffix __=1,$(defines))) \
    $(addprefix -D,$(DEFINES)) \
    -DKERNEL_MAJOR=$(KERNEL_MAJOR) -DKERNEL_MINOR=$(KERNEL_MINOR) \
    -DKERNEL_PATCH=$(KERNEL_PATCH) -DKERNEL_NAME=\"$(KERNEL_NAME)\"

CFLAGS += $(INCLUDES) $(DEFINES)
CPPFLAGS += $(INCLUDES) $(DEFINES)
PPFLAGS += $(INCLUDES) $(DEFINES)

LDFLAGS += -Map $(TMP)/kernel.$(arch).elf.map

CFLAGS += $(FLAGS)
CPPFLAGS += $(FLAGS)

drivers = $(addsuffix .ko,$(addprefix drivers/,$(DYNAMIC_DRIVERS)))

objects	= $(addsuffix .o,$(sources))
objects	:= $(join $(dir $(objects)),$(notdir $(objects)))
objects	:= $(addprefix $(TMP)/,$(objects))
dump	= $(addsuffix .dump,$(objects))
deps	:= $(addsuffix .dep,$(objects))

sources := $(addprefix $(CURDIR)/,$(sources))

all: $(TARGET)

$(drivers):
	@ make -C drivers

$(TARGET): $(objects) $(CURDIR)/arch/$(arch)/kernel.ld $(drivers)
	@ /bin/echo -e "\t LD $(TARGET)  $(RED)"
	@ mkdir -p $(TMP)/arch/$(arch)
	@ $(PP) $(PPFLAGS) -P $(CURDIR)/arch/$(arch)/kernel.ld > $(TMP)/arch/$(arch)/kernel.ld
	@ $(LD) $(LDFLAGS) -T $(TMP)/arch/$(arch)/kernel.ld -o $@ $(objects) \
	$(shell $(CC) -print-libgcc-file-name)
	@ $(DUMP) $(DUMPFLAGS) $@ > $(TMP)/$(notdir $@.dump)
	@ /bin/echo -e -n "$(OK)"

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
	@ make -C drivers clean
	@ rm -rvf $(TMP)

clear: clean
	@ rm -fv $(TARGET)

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

show-tools:
	@ echo "Assembler $(ASM)"
	@ echo "C compiler $(CC)"

-include $(deps)
