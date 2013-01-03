/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\mem.h
    @brief Memory vars
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_MEM_H
#define INCLUDE_SYSTEM_KERNEL_X86_MEM_H

/** @brief Size of a page. Minimum unit of paging */
#define PAGE_SIZE	0x1000 // 4KB
/** @brief Size of stack of kernel tasks */
#define KERNEL_STACK_SIZE  0x1000
/** @brief Size of stack of user tasks */
#define USER_STACK_SIZE    0x1000

#endif
