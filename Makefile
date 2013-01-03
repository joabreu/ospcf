# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: Makefile
# desc: Compilation rules for this folder

include defs.inc

all:
	@echo ">> COMPILING SYSTEM <<"
	@$(MAKE) -C lib
	@$(MAKE) -C system
	@echo ">> All Done."
lib:
	@echo ">> COMPILING LIB <<"
	@$(MAKE) -C lib
	@echo ">> All Done."
system:
	@echo ">> COMPILING SYSTEM <<"
	@$(MAKE) -C system
	@echo ">> All Done."
clean:
	@echo ">> CLEANING <<"
	@$(MAKE) -C lib clean
	@$(MAKE) -C system clean
	@echo ">> All Done."
release:
	@echo ">> CREATING PROJECT RELEASE (see \release folder) << "
	@$(MAKE) -C $(RELEASE_PATH) clean
	@$(MAKE) -C $(RELEASE_PATH) all
	@echo ">> All Done."
