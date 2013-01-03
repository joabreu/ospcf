# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\system.inc
# desc: Basic definitions

include $(SOURCE_PATH)\defs.inc

SYSTEM_INCLUDE_PATH	= -I$(INCLUDE_PATH)\lib \
			  -I$(INCLUDE_PATH)\system \
			  -I$(INCLUDE_PATH) \
			  -I$(LIB_PATH)

CFLAGS += $(SYSTEM_INCLUDE_PATH)
