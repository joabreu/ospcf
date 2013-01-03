# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\drivers\char\keyboard\Makefile
# desc: Compilation rules for this folder

include $(DRIVERS_PATH)\drivers.inc

OBJECTS = kbid.o \
keyboard.o

all: $(OBJECTS)
	@$(LDDRIVERS) $(OBJECTS) $(LDOBJECTS) -o $(BUILTIN) 
clean:
	@$(RM) $(BUILTIN)
	@$(RM) *.o
