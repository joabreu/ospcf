/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\isr.h
    @brief CPU ints
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_ISR_H
#define INCLUDE_SYSTEM_KERNEL_X86_ISR_H

/** @fn void x86IsrInstall()
    @brief Install ISR handler
    @ingroup sys_kernel_arch_x86
*/
void x86IsrInstall();

#endif
