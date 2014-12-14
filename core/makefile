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

KERNEL_MAJOR = 0
KERNEL_MINOR = 1
KERNEL_PATCH = 0
KERNEL_NAME = "Cetraria Islandica"

includes = include
sources = 
defines = KERNEL

ifeq ("$(wildcard ./config.mk)","")
    $(error Configuration file config.mk do not exist)
endif

include config.mk

ifeq ($(arch),)
    $(error Architecture is not specified)
endif

CONFIG = $(addprefix CONFIG_,$(config))

include sources.mk

AR ?= ar
ASM ?= nasm
CC ?= gcc
CPP ?= gcc
DUMP ?= objdump
LD ?= ld
PP ?= cpp

ifneq ($(notdir $(ASM)),nasm)
    $(error $(notdir $(ASM)) assembler is not supported. Use nasm assembler)
endif

config := $(shell echo $(config) | tr 'a-z' 'A-Z')
DEFINES += $(addsuffix =1,$(addprefix CONFIG_,$(config)))
defines += $(arch) $(platform) $(features)

# Temporary directory
TMP = tmp-$(arch)
# Target binary
TARGET = kernel.$(arch).elf

# Reforming lists
INCLUDES    := $(addprefix -I,$(includes))
DEFINES	    := $(addprefix -D__,$(addsuffix __=1,$(defines))) \
    $(addprefix -D,$(DEFINES)) \
    -DKERNEL_MAJOR=$(KERNEL_MAJOR) -DKERNEL_MINOR=$(KERNEL_MINOR) \
    -DKERNEL_PATCH=$(KERNEL_PATCH) -DKERNEL_NAME=\"$(KERNEL_NAME)\"

CFLAGS = $(INCLUDES) $(DEFINES)
CPPFLAGS = $(INCLUDES) $(DEFINES)
PPFLAGS = $(INCLUDES) $(DEFINES)
    	   
# Source flags
	# Enables all warnings
	FLAGS += -Wall 
	# Enable stack protector warnings
	FLAGS += -Wstack-protector
	# Ignore unused attributes
	FLAGS += -Wno-attributes
	# Assert that compilation targets a freestanding environment
	FLAGS += -ffreestanding
	# Do not use the standard system startup files or libraries when linking
	FLAGS += -nostdlib
	# Do not use a so-called “red zone” for x86-64 code
	FLAGS += -mno-red-zone
	# Do not use exceptions
	FLAGS += -fno-exceptions -fno-asynchronous-unwind-tables
	# Do not use run-time type identification features
	CPPFLAGS += -fno-rtti
	# Adding debugging symbols
	FLAGS += -g
	# Using 2011 C ISO standard
	CFLAGS += -std=c11
	CFLAGS += -fplan9-extensions
	
CFLAGS += $(FLAGS)
CPPFLAGS += $(FLAGS)

# Assmbler flags
	# ELF format
	ASMFLAGS += -f elf

# Linker flags
	# Add all symbols to dynamic symbol table
	LDFLAGS += --export-dynamic
	# Print kernel map
	LDFLAGS += -Map $(TMP)/$(TARGET).map

# Dumper flags
	# Disassemble all
	DUMPFLAGS += -D
	# Display all available header information
	DUMPFLAGS += -x
	# Use Intel syntax
	DUMPFLAGS += -M intel

objects	= $(addsuffix .o,$(sources))
objects	:= $(join $(dir $(objects)),$(notdir $(objects)))
objects	:= $(addprefix $(TMP)/,$(objects))
dump	= $(addsuffix .dump,$(objects))
deps	:= $(addsuffix .dep,$(objects))

all: $(TARGET)

$(TARGET): $(objects)
	@ /bin/echo -e "\t LD $(TARGET)  $(RED)"
	@ mkdir -p $(TMP)/arch/$(arch)
	@ $(PP) $(PPFLAGS) -P arch/$(arch)/linker.ld > $(TMP)/arch/$(arch)/linker.ld
	@ $(LD) $(LDFLAGS) -T $(TMP)/arch/$(arch)/linker.ld -o $@ $(objects) \
	$(shell $(CC) -print-libgcc-file-name)
	@ $(DUMP) $(DUMPFLAGS) $@ > $(TMP)/$@.dump
	@ /bin/echo -e -n "$(OK)"
	
$(TMP)/%.c.o: %.c
	@ /bin/echo -e "\t C $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -E $< > $@.E
	@ $(CC) $(CFLAGS) -c -o $@ $<
	@ $(CC) $(CFLAGS) -M -MT $@  $< > $@.dep
	@ $(DUMP) $(DUMPFLAGS) $@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

$(TMP)/%.cpp.o: %.cpp
	@ /bin/echo -e  "\t CPP $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(CPP) $(CPPFLAGS) -c -o $@ $<
	@ $(CPP) -M -MT $@ $(CPPFLAGS) $< > $@.dep
	@ $(CPP) $(CPPFLAGS) -E $< > $@.E
	@ $(DUMP) $(DUMPFLAGS) $(TMP)/$@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

$(TMP)/%.asm.o: %.asm
	@ /bin/echo -e "\t ASM $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(ASM) $(ASMFLAGS) -c -o $@ $<
	@ $(DUMP) $(DUMPFLAGS) $@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

$(TMP)/%.S.o: %.S
	@ /bin/echo -e "\t ASM $(notdir $<) $(notdir $@) $(RED)"
	@ mkdir -p $(dir $@)
	@ $(PP) $(PPFLAGS) $< > $(TMP)/$<.asm
	@ $(PP) -M -MT $@ $(PPFLAGS) $< > $@.dep
	@ $(PP) $(PPFLAGS) -E $< > $@.E
	@ $(ASM) $(ASMFLAGS) -o $@ $(TMP)/$<.asm
	@ $(DUMP) $(DUMPFLAGS) $@ > $@.dump
	@ /bin/echo -e -n "$(OK)"

clean:
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

