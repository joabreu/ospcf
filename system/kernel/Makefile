# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\kernel\Makefile
# desc: Compilation rules for this folder

include $(SYSTEM_PATH)\system.inc

OBJECTS = build.o \
kmain.o \
kparams.o \
ordarray.o \
pcs.o \
pic.o \
queue.o \
rtc.o \
status.o \
syscall.o \
timer.o \
version.o

all: $(OBJECTS)
	@$(MAKE) -C arch all
	@$(MAKE) -C debug all
	@$(MAKE) -C fs all
	@$(MAKE) -C kmodules all
	@$(MAKE) -C memory all
	@$(MAKE) -C process all
	@$(MAKE) -C shell all
	@$(MAKE) -C video all
clean:
	@$(RM) *.o
	@$(MAKE) -C arch clean
	@$(MAKE) -C debug clean
	@$(MAKE) -C fs clean
	@$(MAKE) -C kmodules clean
	@$(MAKE) -C memory clean
	@$(MAKE) -C process clean
	@$(MAKE) -C shell clean
	@$(MAKE) -C video clean
