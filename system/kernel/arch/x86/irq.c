/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\irq.c
    @brief Interrupt Request Routines
*/
#include <errno.h>
#include <kernel\pic.h>
#include <kernel\x86\io.h>
#include <kernel\x86\irq.h>
#include <kernel\x86\idt.h>


/** @brief Pointers to IRQ handlers (if installed) */
static void *InternalIrqHandlers[SYSTEM_IRQ_NENTRIES] = { 0, };

/** @fn extern void x86Irq0AsmHandler()
    @brief Interrupt 0 Handler - Timer Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq0AsmHandler();
/** @fn extern void x86Irq1AsmHandler()
    @brief Interrupt 1 Handler - Keyboard Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq1AsmHandler();
/** @fn extern void x86Irq2AsmHandler()
    @brief Interrupt 2 Handler - Cascade for 8259A Slave controller Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq2AsmHandler();
/** @fn extern void x86Irq3AsmHandler()
    @brief Interrupt 3 Handler - Serial Port 2 Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq3AsmHandler();
/** @fn extern void x86Irq4AsmHandler()
    @brief Interrupt 4 Handler - Serial Port 1 Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq4AsmHandler();
/** @fn extern void x86Irq5AsmHandler()
    @brief Interrupt 5 Handler - AT systems: Parallel Port 2. PS/2 systems: reserved Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq5AsmHandler();
/** @fn extern void x86Irq6AsmHandler()
    @brief Interrupt 6 Handler - Floppy Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq6AsmHandler();
/** @fn extern void x86Irq7AsmHandler()
    @brief Interrupt 7 Handler - Parallel Port 1 Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq7AsmHandler();
/** @fn extern void x86Irq8AsmHandler()
    @brief Interrupt 8 Handler - Real Time Clock(CMOS) Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq8AsmHandler();
/** @fn extern void x86Irq9AsmHandler()
    @brief Interrupt 9 Handler - CGA vertical retrace Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq9AsmHandler();
/** @fn extern void x86Irq10AsmHandler()
    @brief Interrupt 10 Handler - Reserved Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq10AsmHandler();
/** @fn extern void x86Irq11AsmHandler()
    @brief Interrupt 11 Handler - Reserved Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq11AsmHandler();
/** @fn extern void x86Irq12AsmHandler()
    @brief Interrupt 12 Handler - AT systems: reserved. PS/2: auxiliary device Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq12AsmHandler();
/** @fn extern void x86Irq13AsmHandler()
    @brief Interrupt 13 Handler - FPU Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq13AsmHandler();
/** @fn extern void x86Irq14AsmHandler()
    @brief Interrupt 14 Handler - Hard disk controller Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq14AsmHandler();
/** @fn extern void x86Irq15AsmHandler()
    @brief Interrupt 15 Handler - Reserved Interrupt
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Irq15AsmHandler();

int x86IrqInstallHandler(int irq, void (*handler)(x86RegistersStruct *r))
{
     if((irq < 0) || (irq >= SYSTEM_IRQ_NENTRIES))
         return -EINVAL;
          
     if(InternalIrqHandlers[irq] != 0) // Try To Register A Handler Twice !
         return -EENTRYFLL;
     else {
         InternalIrqHandlers[irq] = handler;
         return ESUCCESS;
     }
}
void x86IrqUninstallHandler(int irq)
{
    if((irq < 0) || (irq >= SYSTEM_IRQ_NENTRIES))
         return;
    InternalIrqHandlers[irq] = 0;
}
void x86IrqInstall()
{
    PicRemap(0x20);

    x86IdtSetGate(32, (unsigned)x86Irq0AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(33, (unsigned)x86Irq1AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(34, (unsigned)x86Irq2AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(35, (unsigned)x86Irq3AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(36, (unsigned)x86Irq4AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(37, (unsigned)x86Irq5AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(38, (unsigned)x86Irq6AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(39, (unsigned)x86Irq7AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(40, (unsigned)x86Irq8AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(41, (unsigned)x86Irq9AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(42, (unsigned)x86Irq10AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(43, (unsigned)x86Irq11AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(44, (unsigned)x86Irq12AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(45, (unsigned)x86Irq13AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(46, (unsigned)x86Irq14AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(47, (unsigned)x86Irq15AsmHandler, 0x08, 0x8E);
}
/** @fn void x86IrqHandler(x86RegistersStruct regs)
    @brief IRQ General Handler. This calls the proper IRQ customized handler
    @ingroup sys_kernel_arch_x86 
    @param regs CPU Registers status
    @internal
*/
void x86IrqHandler(x86RegistersStruct regs)
{    
    // This is a blank function pointer
    void (*handler)(x86RegistersStruct *r);

    // Find out if we have a custom handler to run for this
    // IRQ, and then finally, run it
    handler = InternalIrqHandlers[regs.int_no - 32];
    if(handler)
         handler(&regs);
         
    if(regs.int_no >= 40) // Send an EOI to the slave controller
         outportb(0xA0, 0x20);
    outportb(0x20, 0x20); // Send an EOI to the master interrupt
}
