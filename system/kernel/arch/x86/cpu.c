/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\cpu.c
    @brief CPU specific functions
*/
#include <kernel\console.h>
#include <kernel\x86\irq.h>

void x86DumpRegs(x86RegistersStruct *regs)
{
	kprintf("  eip: %08x, useresp: %08x, esp: %08x\n",
    regs->eip, regs->useresp, regs->esp);
	kprintf("  ebx: %08x, edx: %08x, ecx: %08x, eax: %08x\n",
    regs->ebx, regs->edx, regs->ecx, regs->eax);
	kprintf("  edi: %08x, esi: %08x, ebp: %08x, int_no: %d, err_code: %04x\n",
	regs->edi, regs->esi, regs->ebp, regs->int_no, regs->err_code);
	kprintf("  ds: %04x, es: %04x, fs: %04x, gs: %04x, cs: %04x, ss: %04x\n",
    regs->ds, regs->es, regs->fs, regs->gs, regs->cs, regs->ss);
}
