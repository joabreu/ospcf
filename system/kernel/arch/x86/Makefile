# -- PCF Operating System  --
# See [docs\COPYRIGHT.txt] for more info
#
# file: system\kernel\arch\x86\Makefile
# desc: Compilation rules for this folder

include $(SYSTEM_PATH)\system.inc

OBJECTS = __start32.o \
startup.o \
gdt.o \
gdtasm.o \
tss.o \
tssasm.o \
idt.o \
idtasm.o \
io.o \
irq.o \
isr.o \
ints.o \
cpu.o \
paging.o \
pagingasm.o \
pageflt.o \
task.o \
tasks.o

all: $(OBJECTS)
clean:
	@$(RM) *.o
