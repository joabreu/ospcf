# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\block\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

SUBDIRS = floppy
OBJECTS = 
LDOBJECTS:=$(patsubst %, %/$(BUILTIN), $(SUBDIRS))

all: $(OBJECTS)
	@$(MAKE) -C floppy
	@$(LDDRIVERS) $(OBJECTS) $(LDOBJECTS) -o $(BUILTIN)
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
	@$(MAKE) -C floppy clean
