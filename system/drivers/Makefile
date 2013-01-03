# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

SUBDIRS = char video
OBJECTS = 
LDOBJECTS:=$(patsubst %, %/$(BUILTIN), $(SUBDIRS))

all: $(OBJECTS)
	@$(MAKE) -C char
	@$(MAKE) -C video
	@$(LDDRIVERS) $(OBJECTS) $(LDOBJECTS) -o $(BUILTIN) 
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
	@$(MAKE) -C block clean
	@$(MAKE) -C char clean
	@$(MAKE) -C fs clean
	@$(MAKE) -C video clean
