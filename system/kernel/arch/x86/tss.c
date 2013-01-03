/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\tss.c
    @brief Task Segment Selector
*/
#include <string.h>
#include <kernel\x86\tss.h>
#include <kernel\x86\gdt.h>

/** @fn extern void x86TssFlush()
    @brief Instals a new TSS into CPU unsing ltr intruction
    @ingroup sys_kernel_arch_x86 
*/
extern void x86TssFlush();
struct x86TssEntryStruct SystemTssEntry;

/** @fn static void x86WriteTss(int num, unsigned int ss0, unsigned long esp0)
    @brief Inits system TSS and set some values
    @ingroup sys_kernel_arch_x86 
    @param num GDT entry number where this TSS is going to be written
    @param ss0 Value of stack segment (kernel)
    @param esp0 Value of stack pointer (kernel)
    @internal
*/
static void x86WriteTss(int num, unsigned short ss0, unsigned long esp0)
{
    // Calcula o endereço do início e do fim do TSS
    unsigned long base = (unsigned long)&SystemTssEntry;
    unsigned long limit = base + sizeof(struct x86TssEntryStruct);
    
    // Adiciona o TSS ao GDT
    x86GdtSetGate(num, base, limit, 0xE9, 0x00);

    // Reseteta TSS
    memset(&SystemTssEntry, 0, sizeof(struct x86TssEntryStruct));

    SystemTssEntry.ss0  = ss0;  // Set the kernel stack segment.
    SystemTssEntry.esp0 = esp0; // Set the kernel stack pointer.
    
    // Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what 
    // segments should be loaded when the processor switches to kernel mode. Therefore
    // they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
    // but with the last two bits set, making 0x0b and 0x13. The setting of these bits
    // sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
    // to switch to kernel mode from ring 3.
    SystemTssEntry.cs = 0x0b;
    SystemTssEntry.ss = SystemTssEntry.ds = SystemTssEntry.es = SystemTssEntry.fs = SystemTssEntry.gs = 0x13;
}
void x86SetKernelStack(unsigned long stack)
{
    SystemTssEntry.esp0 = stack;
}
void x86TssInit()
{
    x86WriteTss(5, 0x10, 0x0);  // Inits TSS
    x86TssFlush();              // Installs TSS 
}
