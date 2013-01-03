/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\proc.h
    @brief Process Manager
*/
#ifndef INCLUDE_SYSTEM_KERNEL_PROC_H
#define INCLUDE_SYSTEM_KERNEL_PROC_H

#include <kernel\queue.h>
#include <kernel\x86\task.h>

/** @brief Maximum characteres of process name */
#define TASK_NAME_SIZE 31   // 31 Caracteres

/** @brief Process/Thread State: NULL state - neither a process neither null */
#define TS_DUMMY       1
/** @brief Process/Thread State: Ready to run */
#define TS_READY       1
/** @brief Process/Thread State: Running right now */
#define TS_RUNNING     2
/** @brief Process/Thread State: Blocked by something/sleeping */
#define TS_BLOCKED     4
/** @brief Process/Thread State: Ready to be killed */
#define TS_ZOMBIE      5

/** @brief Process/Thread Priority: Very Low */
#define IDLE_PRIO      0
/** @brief Process/Thread Priority: Low */
#define LOW_PRIO       1
/** @brief Process/Thread Priority: Normal */
#define NORMAL_PRIO   2
/** @brief Process/Thread Priority: High */
#define HIGH_PRIO      3
/** @brief Process/Thread Priority: Very High */
#define VERY_HIGH_PRIO 4
/** @brief Process/Thread Priority: Real Time */
#define REAL_TIME_PRIO 5

/** @brief Process Privileges: Kernel Process, Ring 0 */
#define PRIV_KERNEL    0
/** @brief Process Privileges: User Process, Ring 3 */
#define PRIV_USER      1
/** @brief Process Privileges: Nothing */
#define PRIV_DUMMY     2

struct ProcessStruct;

/** @brief Thread structure */
typedef struct ThreadStruct
{
    /** @brief Thread ID (0 - 4.294.967.295)*/
    unsigned long id;
    /** @brief Thread Current state (0 - 255)*/
    unsigned char state;
    /** @brief Process privacy: kernel/user */
    unsigned char privacy;
    /** @brief Process priority */
	unsigned char priority;
    
    /** @brief Parent process */
    struct ProcessStruct *parent;
	/** @brief Thread entry address */
	unsigned long entry;
	/** @brief Architecture dependent fields (i386) */
	struct x86ArchThread ArchThread;
	
	/** @brief Pointer to queue of process's threads */
	QueueStruct *queue;
	
    /** @brief Thread remaining cycles to run */
	unsigned long time;
	/** @brief Thread remaining cycles to wait */
	unsigned long timeout;
	/** @brief User time running (cycles) */
	unsigned long utime;
	/** @brief Supervisor time running  (cycles) */
	unsigned long stime;
	
	/** @brief Current directory vnode */
	//VnodeStruct *CurrentDir;
} ThreadStruct;
/** @brief Process struct */
typedef struct ProcessStruct
{
	/** @brief Process ID */
	unsigned long id;
	/** @brief Process Name */
	char name[TASK_NAME_SIZE + 1];
	/** @brief Process Current state */
	unsigned char state;
	/** @brief Process privacy: kernel/user */
	unsigned char privacy;
	/** @brief Process priority */
	unsigned char priority;
	
	/** @brief Pointer to queue this process is */
	QueueStruct *queue;
	/** @brief List of child processes */
	QueueStruct *children;
	/** @brief Parent process */
	struct ProcessStruct *parent;
	
	/** @brief Number of running threads */
	unsigned long nthreads;
    /** @brief List of process thread's */
	QueueStruct *threads;
	/** @brief Currently running thread */
	ThreadStruct *tcurrent;
	/** @brief main() thread, first thread to be created in process */
	ThreadStruct *tmain;
	
	/** @brief Architecture dependent fields (i386) */
	struct x86ArchProcess ArchProcess;
	
	/** @brief Process remaining cycles to wait */
	unsigned long timeout;
	/** @brief User time running (cycles) */
	unsigned long utime;
	/** @brief Supervisor time running  (cycles) */
	unsigned long stime;
	
	//Files
	// Queue of files
	/** @brief List of opened files */
	//FileStruct *file[MAX_FILE]; // to remove leave the above
	//Consoles	
} ProcessStruct;

ProcessStruct *GetCurrentProcess();
/** @fn int ProcessDisabled()
    @brief Return processing status, if true processing is disabled
    @ingroup sys_kernel_process 
    @return True if processing disabled, 0 otherwise
*/
int ProcessDisabled();
/** @fn void SetProcessingStatus(int disabled)
    @brief Set processing status, disabled=1 disables multitasking
    @ingroup sys_kernel_process 
    @param disabled If true disables multitasking
*/
void SetProcessingStatus(int disabled);
/** @fn unsigned long ProcessCreate(char *name, unsigned long *argv, unsigned char priority, int (*entry)(int argc, char **argv), unsigned char privacy)
    @brief Create a process, create also an initial thread with entry function entry
    @ingroup sys_kernel_process 
    @param name Process name
    @param argv Arguments list of initial thread
    @param priority Process priority ( initial thread priority )
    @param entry Entry function of initial thread
    @param privacy Process privacy ( PRIV_KERNEL or PRIV_USER )
    @return Process ID ( PID )
*/
unsigned long ProcessCreate(char *name, unsigned long *argv, unsigned char priority, int (*entry)(int argc, char **argv), unsigned char privacy);
/** @fn void ProcessYield()
    @brief Re-schedule function. Avoids unnecessary use of CPU cycles. THIS FUNCTION NEEDS REFORMULATION
    @ingroup sys_kernel_process 
*/
void ProcessYield();
/** @fn ProcessStruct *GetProcess(unsigned long pid)
    @brief Returns process struct with designated pid, NULL if process is not present or in zombie queue
    @ingroup sys_kernel_process 
    @param pid Process ID
    @return Process Struct pointer
*/
ProcessStruct *GetProcess(unsigned long pid);
/** @fn void ProcessExit(int result)
    @brief Kills current process and all its threads.
    @ingroup sys_kernel_process 
    @param result Termination error code
*/
void ProcessExit(int result);
/** @fn int ProcessKill(unsigned long pid)
    @brief Kill designated process.
    @ingroup sys_kernel_process 
    @param pid Process id
    @return Error code
*/
int ProcessKill(unsigned long pid);
/** @fn int ProcessSleep(unsigned long timeout)
    @brief Freezes current process for timeout cycles
    @ingroup sys_kernel_process 
    @param timeout Cycles to freeze
    @return Error code
*/
int ProcessSleep(unsigned long timeout);
/** @fn int ProcessWake(ProcessStruct *p)
    @brief Sets process p to state=TS_READY
    @ingroup sys_kernel_process 
    @param p Process struct pointer
    @return Error code
*/
int ProcessWake(ProcessStruct *p);
/** @fn int ProcessWait(unsigned long pid)
    @brief Freezes current process until process pid has ended
    @ingroup sys_kernel_process 
    @param pid Wait until process with this id ends
    @return Error code
*/
int ProcessWait(unsigned long pid);
/** @fn int ProcessRename(char *new_name)
    @brief Rename current process
    @ingroup sys_kernel_process 
    @param new_name Current process new name
    @return Error code
*/
int ProcessRename(char *new_name);
/** @fn ProcessStruct * Schedule()
    @brief Returns next runnable process/thread
    @ingroup sys_kernel_process 
    @return Next runnable process
    @warning Only i386ThreadArchSchedule() should call this function !!!
*/
ProcessStruct * Schedule();
/** @fn int ProcessingInit()
    @brief Init multi-tasking
    @ingroup sys_kernel_process 
    @return Error code
*/
int ProcessingInit();
/** @fn unsigned long ThreadCreate(ProcessStruct *process, void *entry, unsigned long *argv)
    @brief Create a thread in process process
    @ingroup sys_kernel_process 
    @param process Process struct, the new thread will be associated to this
    @param entry Entry function of thread
    @param argv Arguments list
    @return New thread ID or negative error code
*/
unsigned long ThreadCreate(ProcessStruct *process, void *entry, unsigned long *argv);
/** @fn ThreadStruct *GetThread(ProcessStruct *p, unsigned long tid)
    @brief Return thread struct with designated ID
    @ingroup sys_kernel_process 
    @param p Search for threads belonging to this process
    @param tid Thread ID to return
    @return Thread struct or NULL
*/
ThreadStruct *GetThread(ProcessStruct *p, unsigned long tid);
int ThreadKill(ThreadStruct *t);
/** @fn int CountArguments(char **args)
    @brief Return number of arguments in string
    @ingroup sys_kernel_process 
    @param args String
    @return Arguments number or negative error code
*/
int CountArguments(char **args);
/** @fn void ProcessPrintInfo(ProcessStruct *p)
    @brief Print detailed info about a process
    @ingroup sys_kernel_process 
    @param p Process
*/
void ProcessPrintInfo(ProcessStruct *p);
/** @fn void DumpProcesses()
    @brief Print list of present processes
    @ingroup sys_kernel_process
*/
void DumpProcesses();

#endif
