/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\task.c
    @brief Task manager, CPU level functions.
*/
#include <errno.h>
#include <string.h>
#include <kernel\debug.h>
#include <kernel\proc.h>
#include <kernel\x86\tss.h>
#include <kernel\x86\paging.h>
#include <kernel\x86\mem.h>
#include <kernel\mem\kheap.h>
#include <kernel\mem\uheap.h>

/** @brief If defined debug will be outputed */
#define DEBUG_TASK
#ifdef DEBUG_TASK
    /** @brief If debug is selected print a message using the standard DEBUG(...) function */
    #define DM(Msg...) DEBUG(Msg)
#else
    /** @brief Debug is not selected */ 
    #define DM(Msg...) {}
#endif

extern x86PageDirectoryStruct *CurrentDirectory;       // Current CR3 directory
extern ProcessStruct *CurrentProcess;
extern ProcessStruct *Schedule();

/** @fn extern void x86ThreadExitEntry()
    @brief This function is called when a Thread exits

    This function calls the system call SYSTEMCALL_THREAD_EXIT and passes the return value of the thread as an argument
    @ingroup sys_kernel_arch_x86 
*/
extern void x86ThreadExitEntry();

int x86ThreadArchInit(struct x86ArchThread *thread, void *entry, unsigned long *argv, unsigned char priv)
{
    unsigned long *stack;
    int argc = CountArguments((char **)argv);
    argv[argc] = 0; // Set last argument + 1 to NULL
    if(argc < 0)
         argc = 0;
	
    /** @note Following structures are allocated in process' parent:
         - The process struct;
         - The process queue.
    */ 
    thread->kernel_stack_base = (unsigned long)kmalloc_a(KERNEL_STACK_SIZE);
    if(thread->kernel_stack_base == 0)
         return -ENOMEM;
    memset((void *)thread->kernel_stack_base, 0, KERNEL_STACK_SIZE);
    thread->kernel_stack = thread->kernel_stack_base + KERNEL_STACK_SIZE; // Create kernel stack
    if(priv == PRIV_USER) {
         // Apenas aloca memória de user se for preciso
         thread->stack_base = (unsigned long)umalloc_a(USER_STACK_SIZE);
         if(thread->stack_base == 0) {
              kfree((void *)thread->kernel_stack_base);
              return -ENOMEM;
         }
         memset((void *)thread->stack_base, 0, USER_STACK_SIZE);
         thread->stack = thread->stack_base + USER_STACK_SIZE; // Allocate 4 kilobytes of space for user
    } else {
         thread->stack_base = thread->kernel_stack_base;
         thread->stack = thread->kernel_stack;
    }
    stack = (unsigned long*)thread->stack;        // Expand down stack
    
    // We expect all threads to have this layout:
          // ThreadMain(int argc, char **argv);
    // The kernel will not crash if the thread is different (i guess)
    *--stack = (unsigned long)argv; // Argv
    *--stack = argc; // Argc
    *--stack = (unsigned long)&x86ThreadExitEntry;
    
    // Pushed by iret
    if(priv == PRIV_USER) { // User threads have special stack
         *--stack = 0x23;   // SS
         *--stack = thread->stack - 12; // ESP
    }
    *--stack = 0x202;       // EFLAGS
    *--stack = ((priv == PRIV_USER) ? (0x1b) : 0x08);  // CS
    *--stack = (unsigned long)entry;       // EIP
    // Pushed by pusha
    *--stack = 0;           // EDI
    *--stack = 0;           // ESI
    *--stack = 0;           // EBP
    *--stack = 0;           // NULL
    *--stack = 0;           // EBX
    *--stack = 0;           // EDX
    *--stack = 0;           // ECX
    *--stack = 0;           // EAX
    // Pushed by asm handler
    *--stack = ((priv == PRIV_USER) ? (0x23) : 0x10);        // DS
    *--stack = ((priv == PRIV_USER) ? (0x23) : 0x10);        // ES
    *--stack = ((priv == PRIV_USER) ? (0x23) : 0x10);        // FS
    *--stack = ((priv == PRIV_USER) ? (0x23) : 0x10);        // GS
    
    thread->stack = (unsigned long)stack;

    return ESUCCESS;
}
int x86ProcessArchInit(struct x86ArchProcess *proc)
{
    x86PageDirectoryStruct *directory;
    
    //directory = x86ClonePageDirectory(CurrentDirectory);
    directory = CurrentDirectory;
    proc->PageDirectory = directory;

    return ESUCCESS;
}
void x86ThreadArchDestroy(struct x86ArchThread *thread, unsigned char priv)
{
    kfree((void *)thread->kernel_stack_base);
    if(priv == PRIV_USER)
         ufree((void *)thread->stack_base);
}
void x86ProcessArchDestroy(struct x86ArchProcess *proc)
{
    return;
}
/** @fn unsigned long x86ThreadArchSchedule(unsigned long OldEsp)
    @brief Archicheture schedule

    This is the first function executed in the task switching process. Performs operations that are archicheture dependent.
    The current ESP is provided as an argument. The function saves the state of current process and calls the main Schedule() function that returns the process struct pointer to run. After setting some data the function returns the new process ESP to run
    @ingroup sys_kernel_arch_x86 
    @param OldEsp ESP of current process
    @return ESP of new process to run
*/
unsigned long x86ThreadArchSchedule(unsigned long OldEsp)
{
    if(ProcessDisabled())
		 return OldEsp;
		 
    CurrentProcess->tcurrent->ArchThread.stack = OldEsp; // Save old esp
    
    ProcessStruct *to_run = Schedule();
    if(to_run == NULL)
         return OldEsp; // If we do not return a valid esp the kernel will crash
    
    CurrentProcess = to_run;                 // Define new process
    x86SetKernelStack(CurrentProcess->tcurrent->ArchThread.kernel_stack); // Set new kernel esp
    x86SwitchPageDirectory(CurrentProcess->ArchProcess.PageDirectory); // Set Process Page Directory
    return CurrentProcess->tcurrent->ArchThread.stack;         // Return new thread context.
}
