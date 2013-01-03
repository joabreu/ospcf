# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: OSPCF-Source\system\Makefile
# desc: Compilation rules for this folder

include system.inc

SUBDIRS = kernel\arch\x86 \
kernel \
kernel\debug \
kernel\fs \
kernel\memory \
kernel\process \
kernel\shell \
kernel\video
# kernel\devices \
# kernel\kmodules \

OBJECTS:=$(patsubst %,%/*.o, $(SUBDIRS))

BUILTINS = drivers\$(BUILTIN)
MAP	 = bin\sysmap.txt
KRNL_OUT = bin\kernel32
KRNL_LD  = kernel.ld
VERSION	 = kernel\version.o
BUILD	 = $(TOOLS_PATH)\..\build\build

LIBS = # klib\*.a
LIBS+= $(LIB_PATH)\*.a

all: prepare makes link
prepare: 
	$(BUILD)
	@$(RM) $(KRNL_OUT)
	@$(RM) $(VERSION)
makes:
	@echo "> System"
	@echo "  - drivers"
	@$(MAKE) -C drivers
	@echo "  - klib"
	@$(MAKE) -C klib
	@echo "  - kernel"
	@$(MAKE) -C kernel
	@echo "> Done."
link:
	@echo "> Linking ..."
	@$(LD) -static -nostdlib -T $(KRNL_LD) -Map $(MAP) $(OBJECTS) $(BUILTINS) $(LIBS) -o $(KRNL_OUT)
	@echo "> Done."
	@$(XCOPY) $(KRNL_OUT) $(RELEASE_PATH)\kernel32
clean:
	@echo "> Cleaning system"
	@echo "  - drivers"
	@$(MAKE) -C drivers clean
	@echo "  - klib"
	@$(MAKE) -C klib clean
	@echo "  - kernel"
	@$(MAKE) -C kernel clean
	@echo "> Done."
