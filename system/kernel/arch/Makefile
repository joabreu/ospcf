# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\kernel\arch\Makefile
# desc: Compilation rules for this folder

include $(SYSTEM_PATH)\system.inc

OBJECTS = 

all: $(OBJECTS)
	@echo "  - arch -> x86"
	@$(MAKE) -C x86 all
clean:
	@$(RM) *.o
	@echo "  - cleaning arch -> x86"
	@$(MAKE) -C x86 clean
