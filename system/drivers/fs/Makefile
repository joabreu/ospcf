# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\fs\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

SUBDIRS = fat rootfs
OBJECTS = 
LDOBJECTS:=$(patsubst %, %/$(BUILTIN), $(SUBDIRS))

all: $(OBJECTS)
	@$(MAKE) -C fat
	@$(MAKE) -C rootfs
	@$(LDDRIVERS) $(OBJECTS) $(LDOBJECTS) -o $(BUILTIN)
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
	@$(MAKE) -C fat clean
	@$(MAKE) -C rootfs clean
