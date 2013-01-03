/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\kmain.h
    @brief Kernel main loading
*/
#ifndef INCLUDE_SYSTEM_KERNEL_KMAIN_H
#define INCLUDE_SYSTEM_KERNEL_KMAIN_H

#include <kernel\debug.h>

/** @brief Check the result of an init function: if ret is not 0 Crashs the system
    @param x Init function result */
#define INIT(x)	if ((x) < 0) { BUG(); for(;;); }

/** @fn void KernelMain()
    @brief Kernel main function
    @ingroup sys_kernel
*/
void KernelMain();

#endif
