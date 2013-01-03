/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\syscall.c
    @brief System Call
*/
#include <errno.h>
#include <kernel\syscall.h>
#include <kernel\debug.h>
#include <kernel\proc.h>
#include <kernel\pcs.h>
#include <kernel\fs\vfs.h>
#include <kernel\x86\irq.h>
#include <kernel\x86\idt.h>

/** @fn extern void x86SyscallAsmHandler()
    @brief Low level system call handler
    @ingroup sys_kernel
*/
extern void x86SyscallAsmHandler();

/** @brief List of system call's callback's */
SystemCall SystemCallTable[SYSTEMCALL_MAXINDEX];

/** @brief Argument 0 */
#define arg0 (arguments[0])
/** @brief Argument 1 */
#define arg1 (arguments[1])
/** @brief Argument 2 */
#define arg2 (arguments[2])
/** @brief Argument 3 */
#define arg3 (arguments[3])
/** @brief Argument 4 */
#define arg4 (arguments[4])
/** @brief Argument 5 */
#define arg5 (arguments[5])

/** @brief Macro to call a system callback (0 arguments) */
#define SYSTEM_CALL0(arguments, function) function.function0()
/** @brief Macro to call a system callback (1 argument) */
#define SYSTEM_CALL1(arguments, function) function.function1((void *)arg0)
/** @brief Macro to call a system callback (2 arguments) */
#define SYSTEM_CALL2(arguments, function) function.function2((void *)arg0, (void *)arg1)
/** @brief Macro to call a system callback (3 arguments) */
#define SYSTEM_CALL3(arguments, function) function.function3((void *)arg0, (void *)arg1, (void *)arg2)
/** @brief Macro to call a system callback (4 arguments) */
#define SYSTEM_CALL4(arguments, function) function.function4((void *)arg0, (void *)arg1, (void *)arg2, (void *)arg3)
/** @brief Macro to call a system callback (5 arguments) */
#define SYSTEM_CALL5(arguments, function) function.function5((void *)arg0, (void *)arg1, (void *)arg2, (void *)arg3, (void *)arg4)
/** @brief Macro to call a system callback (6 arguments) */
#define SYSTEM_CALL6(arguments, function) function.function6((void *)arg0, (void *)arg1, (void *)arg2, (void *)arg3, (void *)arg4, (void *)arg5)

/** @fn void SyscallHandler(x86RegistersStruct r)
    @brief System call general handler
    @ingroup sys_kernel
    @param r CPU registers status
*/
void SyscallHandler(x86RegistersStruct r)
{
    signed long int index;
	signed char **arguments;
	signed long int *ret;
    signed long int ret_aux = 0;

	ret = (signed long int*)r.ecx;
	arguments = (signed char **)r.ebx;
	index = r.eax;

    if(!ret)       // We have no return address, use an auxiliary
    {
         DEBUG("Ret not assigned (ecx=0x%x), allocating for syscall %d", r.ecx, r.eax);
         ret = &ret_aux;
    } 
    if(SystemCallTable[index].function.function != 0)
    {
         switch(SystemCallTable[index].params)
         {
              case 0:
                   *ret = SYSTEM_CALL0(arguments, SystemCallTable[index].function);
                   break;
              case 1:
                   *ret = SYSTEM_CALL1(arguments, SystemCallTable[index].function);
                   break;
              case 2:
                   *ret = SYSTEM_CALL2(arguments, SystemCallTable[index].function);
                   break;
              case 3:
                   *ret = SYSTEM_CALL3(arguments, SystemCallTable[index].function);
                   break;
              case 4:
                   *ret = SYSTEM_CALL4(arguments, SystemCallTable[index].function);
                   break;
              case 5:
                   *ret = SYSTEM_CALL5(arguments, SystemCallTable[index].function);
                   break;
              case 6:
                   *ret = SYSTEM_CALL6(arguments, SystemCallTable[index].function);
              default:
                   *ret = -1;
                   // Unsupported Parameters
                   break;
         }    
    }
}
int SyscallAdd(int index, int params, void *function)
{
    if(index > SYSTEMCALL_NOCALL || index < SYSTEMCALL_MAXINDEX)
    {
         SystemCallTable[index].function.function = function;
         SystemCallTable[index].params = params;
         return ESUCCESS;
    }
    return -EINVAL;
}
int SyscallRemove(int index)
{
    if(index > SYSTEMCALL_NOCALL || index < SYSTEMCALL_MAXINDEX)
    {
         SystemCallTable[index].function.function = NULL;
         SystemCallTable[index].params = 0;
         return ESUCCESS;
    }
    return -EINVAL;
}
void SyscallTest(int arg)
{
    PcsSound(arg);
    kprintf("hello from %d\n", arg);
}
void SyscallInstall()
{
    int i = 0;
	for(i=SYSTEMCALL_NOCALL; i<SYSTEMCALL_MAXINDEX; i++)
	{	
		SystemCallTable[i].function.function = NULL;
		SystemCallTable[i].params = 0;
	}
	SyscallAdd(SYSTEMCALL_THREAD_EXIT, 1, ProcessExit);
	SyscallAdd(SYSTEMCALL_READ, 3, VfsRead);
	SyscallAdd(SYSTEMCALL_WRITE, 3, VfsWrite);
	SyscallAdd(SYSTEMCALL_TEST, 1, SyscallTest);
	
	x86IdtSetGate(128, (unsigned)x86SyscallAsmHandler, 0x08, 0x8E);
}
