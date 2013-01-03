/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\syscall.h
    @brief System Call
*/
#ifndef INCLUDE_SYSTEM_KERNEL_SYSCALL_H
#define INCLUDE_SYSTEM_KERNEL_SYSCALL_H

#include <sys\syscall.h>

/** @brief System Call structure: no params */
typedef int (*TypeSyscall0)();
/** @brief System Call structure: 1 param */
typedef int (*TypeSyscall1)(void *);
/** @brief System Call structure: 2 params */
typedef int (*TypeSyscall2)(void *, void *);
/** @brief System Call structure: 3 params */
typedef int (*TypeSyscall3)(void *, void *, void *);
/** @brief System Call structure: 4 params */
typedef int (*TypeSyscall4)(void *, void *, void *, void *);
/** @brief System Call structure: 5 params */
typedef int (*TypeSyscall5)(void *, void *, void *, void *, void *);
/** @brief System Call structure: 6 params */
typedef int (*TypeSyscall6)(void *, void *, void *, void *, void *, void *);

/** @brief System Call Function Structure */
typedef struct _SystemCallFunction
{
    union
	{
		void *function;
		TypeSyscall0 function0;
		TypeSyscall1 function1;
		TypeSyscall2 function2;
		TypeSyscall3 function3;
		TypeSyscall4 function4;
		TypeSyscall5 function5;
		TypeSyscall6 function6;
	};
} SystemCallFunction;

/** @brief System Call Structure */
typedef struct _SystemCall
{
	/** @brief Number of syscall params */
	int params;
	/** @brief System call function pointer */
	SystemCallFunction function;
} SystemCall;

/** @fn int SyscallAdd(int index, int params, void *function)
    @brief Adds a syscall into the active table
    @ingroup sys_kernel
    @param index Syscall index, see <sys\syscall.h>
    @param params Number of syscall params
    @param function Function pointer to syscall
    @return Error code
*/
int SyscallAdd(int index, int params, void *function);
/** @fn int SyscallRemove(int index)
    @brief Remove a syscall from the active table
    @ingroup sys_kernel
    @param index Syscall index, see <sys\syscall.h>
    @return Error code
*/
int SyscallRemove(int index);
/** @fn void SyscallInstall()
    @brief Install system calls
    @ingroup sys_kernel
*/
void SyscallInstall();

#endif
