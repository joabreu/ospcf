/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\isr.c
    @brief Interrupt Service Routines
*/
#include <kernel\debug.h>
#include <kernel\console.h>
#include <kernel\x86\idt.h>
#include <kernel\x86\isr.h>
#include <kernel\x86\irq.h>
#include <kernel\x86\paging.h>

/** @brief Different Descriptions for exceptions */
static unsigned char *ExceptionMessages[] =
{
    "Division By Zero",             // 0
    "Debug",                        // 1
    "Non Maskable Interrupt",       // 2
    "Breakpoint",                   // 3
    "INTO Detected Overflow",       // 4
    "Out of Bounds",                // 5
    "Invalid Opcode",               // 6
    "Device Not Available",         // 7

    "Double Fault",                 // 8
    "Coprocessor Segment Overrun",  // 9
    "Invalid TSS",                  // 10
    "Segment Not Present",          // 11
    "Stack Fault",                  // 12
    "General Protection Fault",     // 13
    "Page Fault",                   // 14
    "Unknown",                      // 15, This is not on Intel manuals :O

    "X87 FPU Floating Point Error", // 16
    "Alignment Check",              // 17
    "Machine Check",                // 18
    "SIMD Floating-Point",          // 19
    "Reserved 20",                  // 20
    "Reserved 21",                  // 21
    "Reserved 22",                  // 22
    "Reserved 23",                  // 23

    "Reserved 24",                  // 24
    "Reserved 25",                  // 25
    "Reserved 26",                  // 26
    "Reserved 27",                  // 27
    "Reserved 28",                  // 28
    "Reserved 29",                  // 29
    "Reserved 30",                  // 30
    "Reserved 31"                   // 31
};

/** @fn extern void x86Isr0AsmHandler()
    @brief Exception 0 Handler - Division By Zero Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr0AsmHandler();
/** @fn extern void x86Isr1AsmHandler()
    @brief Exception 1 Handler - Debug Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr1AsmHandler();
/** @fn extern void x86Isr2AsmHandler()
    @brief Exception 2 Handler - Non Maskable Interrupt Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr2AsmHandler();
/** @fn extern void x86Isr3AsmHandler()
    @brief Exception 3 Handler - Breakpoint Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr3AsmHandler();
/** @fn extern void x86Isr4AsmHandler()
    @brief Exception 4 Handler - INTO Detected Overflow Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr4AsmHandler();
/** @fn extern void x86Isr5AsmHandler()
    @brief Exception 5 Handler - Out of Bounds Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr5AsmHandler();
/** @fn extern void x86Isr6AsmHandler()
    @brief Exception 6 Handler - Invalid Opcode Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr6AsmHandler();
/** @fn extern void x86Isr7AsmHandler()
    @brief Exception 7 Handler - Device Not Available Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr7AsmHandler();
/** @fn extern void x86Isr8AsmHandler()
    @brief Exception 8 Handler - Double Fault Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr8AsmHandler();
/** @fn extern void x86Isr9AsmHandler()
    @brief Exception 9 Handler - Coprocessor Segment Overrun Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr9AsmHandler();
/** @fn extern void x86Isr10AsmHandler()
    @brief Exception 10 Handler - Invalid TSS Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr10AsmHandler();
/** @fn extern void x86Isr11AsmHandler()
    @brief Exception 11 Handler - Segment Not Present Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr11AsmHandler();
/** @fn extern void x86Isr12AsmHandler()
    @brief Exception 12 Handler - Stack Fault Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr12AsmHandler();
/** @fn extern void x86Isr13AsmHandler()
    @brief Exception 13 Handler - General Protection Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr13AsmHandler();
/** @fn extern void x86Isr14AsmHandler()
    @brief Exception 14 Handler - Page Fault Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr14AsmHandler();
/** @fn extern void x86Isr15AsmHandler()
    @brief Exception 15 Handler - Unknown Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr15AsmHandler();
/** @fn extern void x86Isr16AsmHandler()
    @brief Exception 16 Handler - X87 FPU Floating Point Error Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr16AsmHandler();
/** @fn extern void x86Isr17AsmHandler()
    @brief Exception 17 Handler - Alignment Check Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr17AsmHandler();
/** @fn extern void x86Isr18AsmHandler()
    @brief Exception 18 Handler - Machine Check Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr18AsmHandler();
/** @fn extern void x86Isr19AsmHandler()
    @brief Exception 19 Handler - SIMD Floating-Point Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr19AsmHandler();
/** @fn extern void x86Isr20AsmHandler()
    @brief Exception 20 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr20AsmHandler();
/** @fn extern void x86Isr21AsmHandler()
    @brief Exception 21 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr21AsmHandler();
/** @fn extern void x86Isr22AsmHandler()
    @brief Exception 22 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr22AsmHandler();
/** @fn extern void x86Isr23AsmHandler()
    @brief Exception 23 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr23AsmHandler();
/** @fn extern void x86Isr24AsmHandler()
    @brief Exception 24 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr24AsmHandler();
/** @fn extern void x86Isr25AsmHandler()
    @brief Exception 25 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr25AsmHandler();
/** @fn extern void x86Isr26AsmHandler()
    @brief Exception 26 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr26AsmHandler();
/** @fn extern void x86Isr27AsmHandler()
    @brief Exception 27 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr27AsmHandler();
/** @fn extern void x86Isr28AsmHandler()
    @brief Exception 28 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr28AsmHandler();
/** @fn extern void x86Isr29AsmHandler()
    @brief Exception 29 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr29AsmHandler();
/** @fn extern void x86Isr30AsmHandler()
    @brief Exception 30 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr30AsmHandler();
/** @fn extern void x86Isr31AsmHandler()
    @brief Exception 31 Handler - Reserved Exception
    @ingroup sys_kernel_arch_x86 
    @internal
*/
extern void x86Isr31AsmHandler();


void x86IsrInstall()
{
    //                                  |- Present, Ring 0, lower 5 = 24(E)
    x86IdtSetGate(0, (unsigned)x86Isr0AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(1, (unsigned)x86Isr1AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(2, (unsigned)x86Isr2AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(3, (unsigned)x86Isr3AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(4, (unsigned)x86Isr4AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(5, (unsigned)x86Isr5AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(6, (unsigned)x86Isr6AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(7, (unsigned)x86Isr7AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(8, (unsigned)x86Isr8AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(9, (unsigned)x86Isr9AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(10, (unsigned)x86Isr10AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(11, (unsigned)x86Isr11AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(12, (unsigned)x86Isr12AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(13, (unsigned)x86Isr13AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(14, (unsigned)x86Isr14AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(15, (unsigned)x86Isr15AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(16, (unsigned)x86Isr16AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(17, (unsigned)x86Isr17AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(18, (unsigned)x86Isr18AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(19, (unsigned)x86Isr19AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(20, (unsigned)x86Isr20AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(21, (unsigned)x86Isr21AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(22, (unsigned)x86Isr22AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(23, (unsigned)x86Isr23AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(24, (unsigned)x86Isr24AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(25, (unsigned)x86Isr25AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(26, (unsigned)x86Isr26AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(27, (unsigned)x86Isr27AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(28, (unsigned)x86Isr28AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(29, (unsigned)x86Isr29AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(30, (unsigned)x86Isr30AsmHandler, 0x08, 0x8E);
    x86IdtSetGate(31, (unsigned)x86Isr31AsmHandler, 0x08, 0x8E);
}
/** @fn void x86IsrFaultHandler(x86RegistersStruct regs)
    @brief ISR internal handler
    @ingroup sys_kernel_arch_x86
    @param regs CPU registers
    @internal
*/
void x86IsrFaultHandler(x86RegistersStruct regs)
{
    unsigned char int_no = regs.int_no & 0xFF;

    switch(int_no) {
         case 14:
              x86PageFaultHandler(&regs);
              break;
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 7:
         case 8:
         case 9:
         case 10:
         case 11:
         case 12:
         case 13:
         case 15:
         case 16:
         case 17:
         case 18:
         case 19:
         case 20:
         case 21:
         case 22:
         case 23:
         case 24:
         case 25:
         case 26:
         case 27:
         case 28:
         case 29:
         case 30:
         case 31:
              RPANIC(&regs, ExceptionMessages[int_no]);
              break;
         default:
              RPANIC(&regs, "Unhandled exception:%d", int_no);
              break;
    }
    while(1)
         ;
}
