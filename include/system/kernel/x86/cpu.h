/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\cpu.h
    @brief CPU functions
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_CPU_H
#define INCLUDE_SYSTEM_KERNEL_X86_CPU_H

#include <kernel\x86\irq.h>

/** @brief Disable interrupts */
#define CLI() __asm__ __volatile__("cli")
/** @brief Enable interrupts */
#define STI() __asm__ __volatile__("sti")
/** @brief Save current CPU status
    @param w CPU status 
*/
#define SAVEFLAGS(w) __asm__ __volatile__ ("pushfl; popl %0":"=r" (w)::"memory")
/** @brief Restore previousily saved status
    @param w CPU status
*/
#define RESTOREFLAGS(w) __asm__ __volatile__ ("pushl %0; popfl"::"r" (w):"memory")
/** @brief Enter critical region. Save CPU status and disables interrupts
    @param x CPU status
*/
#define INTFLGS_ENTER(x)	SAVEFLAGS(x);CLI()
/** @brief Rstore previous state and enables interrupts
    @param x CPU status
*/
#define INTFLGS_LEAVE(x)	RESTOREFLAGS(x)

/** @fn void x86DumpRegs(x86RegistersStruct *regs)
    @brief Output registers values
    @ingroup sys_kernel_arch_x86
    @param regs CPU registers
    @todo Move this to debug
*/
void x86DumpRegs(x86RegistersStruct *regs);

#endif
