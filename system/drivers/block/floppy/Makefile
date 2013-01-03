# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\block\floppy\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

OBJECTS = 

all: $(OBJECTS)
	@$(LDDRIVERS) $(OBJECTS) -o $(BUILTIN) 
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
