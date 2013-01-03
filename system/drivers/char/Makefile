# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\char\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

SUBDIRS = keyboard serial
OBJECTS = 
LDOBJECTS:=$(patsubst %, %/$(BUILTIN), $(SUBDIRS))

all: $(OBJECTS)
	@$(MAKE) -C keyboard
	@$(MAKE) -C serial
	@$(LDDRIVERS) $(OBJECTS) $(LDOBJECTS) -o $(BUILTIN) 
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
	@$(MAKE) -C keyboard clean
	@$(MAKE) -C serial clean
