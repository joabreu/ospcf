/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\irq.h
    @brief Hardware Ints
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_IRQ_H
#define INCLUDE_SYSTEM_KERNEL_X86_IRQ_H

/** @brief Number of IRQ entries used by the system */
#define SYSTEM_IRQ_NENTRIES 16

/** @brief List of CPU registers passed by the asm handler */
typedef struct
{
    /** @brief Pushed by asm handler: CPU GS register value */
    unsigned int gs;
    /** @brief Pushed by asm handler: CPU FS register value */
    unsigned int fs;
    /** @brief Pushed by asm handler: CPU ES register value */
    unsigned int es;
    /** @brief Pushed by asm handler: CPU DS register value */
    unsigned int ds;
    
    /** @brief Pushed by pusha: CPU EDI register value */
    unsigned int edi;
    /** @brief Pushed by pusha: CPU ESI register value */
    unsigned int esi;
    /** @brief Pushed by pusha: CPU EBP register value */
    unsigned int ebp;
    /** @brief Pushed by pusha: CPU ESP register value */
    unsigned int esp;
    /** @brief Pushed by pusha: CPU EBX register value */
    unsigned int ebx;
    /** @brief Pushed by pusha: CPU EDX register value */
    unsigned int edx;
    /** @brief Pushed by pusha: CPU ECX register value */
    unsigned int ecx;
    /** @brief Pushed by pusha: CPU EAX register value */
    unsigned int eax;
    
    /** @brief Pushed by pre-asm handler: IRQ number */
    unsigned int int_no;
    /** @brief Pushed by pre-asm handler: Error code (if any) */
    unsigned int err_code;
    
    /** @brief Pushed by iret: EIP before IRQ fired */
    unsigned int eip;
    /** @brief Pushed by iret: CS register value */
    unsigned int cs;
    /** @brief Pushed by iret: EFLAGS */
    unsigned int eflags;
    /** @brief Pushed by iret: User ESP if process privacy is user */
    unsigned int useresp;
    /** @brief Pushed by iret: SS register value */
    unsigned int ss;
} x86RegistersStruct;

/** @fn int x86IrqInstallHandler(int irq, void (*handler)(x86RegistersStruct *r))
    @brief Install customized handler to designated interrupt
    @ingroup sys_kernel_arch_x86
    @param irq IRQ number
    @param handler Customized handler
    @return Error code
*/
int x86IrqInstallHandler(int irq, void (*handler)(x86RegistersStruct *r));
/** @fn void x86IrqUninstallHandler(int irq)
    @brief Uninstall customized handler
    @ingroup sys_kernel_arch_x86
    @param irq IRQ number
*/
void x86IrqUninstallHandler(int irq);
/** @fn void x86IrqInstall()
    @brief Install IRQ general handler
    @ingroup sys_kernel_arch_x86
*/
void x86IrqInstall();

#endif
