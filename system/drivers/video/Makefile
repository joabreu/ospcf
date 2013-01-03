# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\video\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

SUBDIRS = vga
OBJECTS = 
LDOBJECTS:=$(patsubst %, %/$(BUILTIN), $(SUBDIRS))

all: $(OBJECTS)
	@$(MAKE) -C vga
	@$(LDDRIVERS) $(OBJECTS) $(LDOBJECTS) -o $(BUILTIN) 
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
	@$(MAKE) -C vga clean
