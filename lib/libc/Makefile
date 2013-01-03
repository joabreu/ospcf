# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: lib\libc\Makefile
# desc: Compilation rules for this folder

include $(LIB_PATH)\lib.inc

SUBDIRS = os assert ctype errno math stdio stdlib string time sys
OBJECTS:=$(patsubst %,%/*.o, $(SUBDIRS))

all: 
	@echo "  - libc"
	@$(MAKE) -C os
	@$(MAKE) -C assert
	@$(MAKE) -C ctype
	@$(MAKE) -C errno
	@$(MAKE) -C float
	@$(MAKE) -C limits
	@$(MAKE) -C locale
	@$(MAKE) -C math
	@$(MAKE) -C setjmp
	@$(MAKE) -C signal
	@$(MAKE) -C stdarg
	@$(MAKE) -C stddef
	@$(MAKE) -C stdio
	@$(MAKE) -C stdlib
	@$(MAKE) -C string
	@$(MAKE) -C time
	@$(MAKE) -C sys
	$(AR) $(ARFLAGS) $(LIB_TARGET)\libc$(LIB_EXTENSION) $(OBJECTS)
clean:
	@echo "  - cleaning libc"
	@$(RM) *.o $(LIB_TARGET)\libc$(LIB_EXTENSION)
	@$(MAKE) -C os clean
	@$(MAKE) -C assert clean
	@$(MAKE) -C ctype clean
	@$(MAKE) -C errno clean
	@$(MAKE) -C float clean
	@$(MAKE) -C limits clean
	@$(MAKE) -C locale clean
	@$(MAKE) -C math clean
	@$(MAKE) -C setjmp clean
	@$(MAKE) -C signal clean
	@$(MAKE) -C stdarg clean
	@$(MAKE) -C stddef clean
	@$(MAKE) -C stdio clean
	@$(MAKE) -C stdlib clean
	@$(MAKE) -C string clean
	@$(MAKE) -C time clean
	@$(MAKE) -C sys clean
