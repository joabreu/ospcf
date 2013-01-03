# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: lib\lib.inc
# desc: Basic definitions

include $(SOURCE_PATH)\defs.inc

LIB_TARGET = $(LIB_PATH)
LIB_EXTENSION = .a
LIB_INCLUDE_PATH	= -I$(INCLUDE_PATH)\lib \
			  -I$(INCLUDE_PATH) \
			  -I$(LIB_PATH)\include

CFLAGS += $(LIB_INCLUDE_PATH)
