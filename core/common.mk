#**
#* @file    common.mk
#* @brief   Common definitions
#* @version 0.1
#* @date    23.06.2015.
#*
#* @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
#* @copyright see file LICENSE
#*

KERNEL_MAJOR = 0
KERNEL_MINOR = 1
KERNEL_PATCH = 0
KERNEL_NAME = "Cetraria Islandica"

AR ?= ar
ASM ?= nasm
CC ?= gcc
CPP ?= gcc
DUMP ?= objdump
LD ?= ld
PP ?= cpp

# Source flags
	# Enables all warnings
	FLAGS = -Wall
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

# Assmbler flags
	# ELF format
	ASMFLAGS = -f elf

# Linker flags
	# Add all symbols to dynamic symbol table
	LDFLAGS = --export-dynamic

# Dumper flags
	# Disassemble all
	DUMPFLAGS = -D
	# Display all available header information
	DUMPFLAGS += -x
	# Use Intel syntax
	DUMPFLAGS += -M intel
