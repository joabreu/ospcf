# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\klib\Makefile
# desc: Compilation rules for this folder

include $(SYSTEM_PATH)\system.inc

SUBDIRS = stdio string
OBJECTS = 
LDOBJECTS:=$(patsubst %, %/$(BUILTIN), $(SUBDIRS))

all: $(OBJECTS)
#	@$(MAKE) -C stdio
#	@$(MAKE) -C string
#	$(AR) $(ARFLAGS) klib.a $(OBJECTS) $(LDOBJECTS)
clean:
	@$(RM) *.o klib.a
	@$(MAKE) -C stdio clean
	@$(MAKE) -C string clean
