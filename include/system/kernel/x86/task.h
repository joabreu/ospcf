/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\task.h
    @brief Tasks management functions
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_TASK_H
#define INCLUDE_SYSTEM_KERNEL_X86_TASK_H

#include <kernel\x86\paging.h>

/** @brief Architecture Specific Thread struct */
struct x86ArchThread
{
    /** @brief Process stack current status.

    This can point to either kernel_stack (if process type is Kernel) or to a User space adddress */ 
    unsigned long stack;
    /** @brief Process stack base for kfree(...) or ufree(...) */ 
    unsigned long stack_base; 
    /** @brief Kernel stack pointer */
    unsigned long kernel_stack;
    /** @brief Kernel stack base for kfree(...) */
    unsigned long kernel_stack_base;
};
/** @brief Architecture Specific Process struct */
struct x86ArchProcess
{
    /** @brief Process Page Directory */
    x86PageDirectoryStruct *PageDirectory;
};

/** @fn int x86ThreadArchInit(struct x86ArchThread *thread, void *entry, unsigned long *argv, unsigned char priv)
    @brief Set architecture dependent settings for a thread
    @ingroup sys_kernel_arch_x86 
    @param thread Pointer to thread struct
    @param entry Thread entry function
    @param argv Arguments to Entry(int argc, char **argv)
    @param priv Thread privileges
    @return Error Code
*/
int x86ThreadArchInit(struct x86ArchThread *thread, void *entry, unsigned long *argv, unsigned char priv);
/** @fn int x86ProcessArchInit(struct x86ArchProcess *proc)
    @brief Set architecture dependent settings for a process
    @ingroup sys_kernel_arch_x86 
    @param proc Pointer process struct
    @return Error Code
*/
int x86ProcessArchInit(struct x86ArchProcess *proc);
/** @fn void x86ThreadArchDestroy(struct x86ArchThread *thread, unsigned char priv)
    @brief Free the memory allocated for Arch dependent settings (stacks) [Thread's]
    @ingroup sys_kernel_arch_x86 
    @param thread Thread Struct
    @param priv Thread privacy
*/
void x86ThreadArchDestroy(struct x86ArchThread *thread, unsigned char priv);
/** @fn void x86ProcessArchDestroy(struct x86ArchProcess *proc)
    @brief Free the memory allocated for Arch dependent settings (page directory) [Processes]
    @ingroup sys_kernel_arch_x86 
    @param proc Process Struct
*/
void x86ProcessArchDestroy(struct x86ArchProcess *proc);

#endif
