/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\pageflt.c
    @brief Page fault handler
*/
#include <stdio.h>
#include <kernel\debug.h>
#include <kernel\proc.h>
#include <kernel\x86\irq.h>
#include <kernel\x86\cpu.h>

void x86PageFaultHandler(x86RegistersStruct *regs)
{
    CLI();
    ProcessStruct *p = GetCurrentProcess();
    
    // Get address that caused the Page Fault, stored in CR2
    unsigned long faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // Calculates error code
    int present   = !(regs->err_code & 0x1); // Page not present
    int rw = regs->err_code & 0x2;           // Write operation?
    int us = regs->err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?

    kprintfc(COLOUR_WHITE, "\nPAGE FAULT ! ( %s%s%s%s ) at 0x%x - EIP: %x\n",
         ((present) ? "present " : ""),
         ((rw) ? "read-only " : ""),
         ((us) ? "user-mode " : ""),
         ((reserved) ? "reserved " : ""),
         faulting_address, regs->eip);
    kprintfc(COLOUR_WHITE, "Current Process: pid=%ld, name='%s'\n\n", p->id, p->name);
    DEBUG("PAGE FAULT ! ( %s%s%s%s ) at 0x%x - EIP: %x",
         ((present) ? "present " : ""),
         ((rw) ? "read-only " : ""),
         ((us) ? "user-mode " : ""),
         ((reserved) ? "reserved " : ""),
         faulting_address, regs->eip);
    DEBUG("Current Process: pid=%ld, name='%s'", p->id, p->name);
        
    // Clean error code so that we can return to normal execution
    __asm__ (
		 "movl %0, %%eax \t\n"
		 "movl %%eax, %%cr2\t\n"
		 :
		 : "g" (0x0)
		 );
    // It is now safe to enable interrupts 
    
    STI();
    while(1)
         ;//KILL PROCESS
}
