# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: defs.inc
# desc: Basic definitions

MAKE = c:\djgpp\bin\make -s
BUILTIN	= builtin.o

# Used software
C	= gcc
CPP	= g++
AS	= as
ASM	= nasm
AR	= ar
LD	= ld-elf
COPY	= copy
XCOPY	= xcopy /Y
MCOPY	= $(TOOLS_PATH)\mcopy
MMD	= $(TOOLS_PATH)\mmd
RM	= del

CFLAGS	= -Wall -nostdlib -nostdinc -fno-builtin
ARFLAGS	= rcs
ASMFLAGS = -f elf


# Compilation rules
%.o:%.s
	@echo (S)   $<
	@$(AS) $< -o $@
%.o: %.c
	@echo (C)   $<
	@$(C) $(CFLAGS) -c $< -o $@
%.o: %.asm
	@echo (ASM) $<
	@$(ASM) $(ASMFLAGS) $< -o $@
