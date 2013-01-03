/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\process\process.c
    @brief Process manager
*/
#include <errno.h>
#include <string.h>
#include <kernel\debug.h>
#include <kernel\proc.h>
#include <kernel\mem\kheap.h>
#include <kernel\x86\cpu.h>
#include <kernel\x86\idt.h>

/** @brief If defined debug will be outputed */
#define DEBUG_PROC
#ifdef DEBUG_PROC
  /** @brief If debug is selected print a message using the standard DEBUG(...) function */
  #define DM(Msg...) DEBUG(Msg)
#else
  /** @brief Debug is not selected */ 
  #define DM(Msg...) {}
#endif

/** @fn extern void x86Irq17AsmHandler()
    @brief Low level irq handler. Re-schedule int
    @ingroup sys_kernel_process
*/
extern void x86Irq17AsmHandler();

/** @brief Ready to run processes */
QueueStruct *RunQueue = 0;
/** @brief Sleeping processes */
QueueStruct *WaitQueue = 0;
/** @brief Ready to be killed processes */
QueueStruct *ZombieQueue = 0;
/** @brief Running process pointer */   
ProcessStruct *CurrentProcess;
//
/** @brief Set to 1 to disable task switching, 0 to enable */
static int _TaskingDisabled = 1;
/** @brief Next valid Process ID */
static unsigned long NextPID;

/** @fn int DummyThread(int argc, char **argv)
    @brief Dummy Thread to initializate process manager
    @param argc Argument count
    @param argv Arguments
    @ingroup sys_kernel_process
    @return Error Code
*/
int DummyThread(int argc, char **argv)
{
    return ESUCCESS;
}
ProcessStruct *GetCurrentProcess()
{
	return CurrentProcess;
}
int ProcessDisabled()
{
    return _TaskingDisabled;
}
void SetProcessingStatus(int disabled)
{
    _TaskingDisabled = disabled;
}
/** @fn static unsigned long ProcessInit(ProcessStruct *new, char *name, unsigned char priority, unsigned char privacy)
    @brief Inits a process. Called from ProcessCreate(...)
    @ingroup sys_kernel_process 
    @param new New process Struct
    @param name New process name
    @param priority New process priority
    @param privacy New process privacy
    @return New process ID or negative Error Code
    @internal
*/
static unsigned long ProcessInit(ProcessStruct *new, char *name, unsigned char priority, unsigned char privacy)
{
	ProcessStruct *parent;
	parent = GetCurrentProcess();
	
    int ret = x86ProcessArchInit(&new->ArchProcess); // Init arch dependent settings
    if(ret != ESUCCESS) {
         DM("Failed initing process arch (errno:%d)", ret);
         return -EIFAIL;
    }
    new->id = NextPID++; // Process ID
	strncpy(new->name, name, TASK_NAME_SIZE); // Process Name
	new->name[TASK_NAME_SIZE+1] = '\0';
	new->state = TS_READY; // Process State
	new->privacy = privacy; // Process privileges (kernel/user)
	new->priority = priority; // Prioridade
	new->parent = parent;
	
    // Aloca uma queue
	new->queue = (QueueStruct *)kmalloc(sizeof(QueueStruct));
	if(new->queue == NULL) {
		 x86ProcessArchDestroy(&new->ArchProcess);
         return -ENOMEM;
	}
	memset(new->queue, 0, sizeof(QueueStruct));
   
    // Init internal queue
	new->queue->next= NULL;
	new->queue->prev= NULL;
	new->queue->data = (void *)new;
	
    DM("new process created (at 0x%x):\n\
    pid=%ld | name='%s'\n\
    privacy=%d | priority=%ld\n\
    queue=0x%x", new, new->id, new->name, new->privacy, new->priority, new->queue);
	return new->id;
}
unsigned long ProcessCreate(char *name, unsigned long *argv, unsigned char priority, 
                  int (*entry)(int argc, char **argv), unsigned char privacy)
{
    ProcessStruct *process;
    
	process = (ProcessStruct *)kmalloc(sizeof(ProcessStruct)); // Alloc process struct
	if(process == NULL)
         return -ENOMEM;
 
    memset(process, 0, sizeof(ProcessStruct)); // Zero the process struct
    unsigned long ret = ProcessInit(process, name, priority, privacy); // Init process
    if(ret < 0) {
         kfree(process);
         DM("Failed initing process (errno:%ld)", ret);
         return -EIFAIL;
    }
    // Create here the new thread
    unsigned long tret = ThreadCreate(process, entry, argv);
    if(tret < 0) {
         x86ProcessArchDestroy(&process->ArchProcess);
		 kfree(process->queue);
		 kfree(process);
     	 DM("Failed initing first thread (errno:%ld)", tret);
         return -EIFAIL;
    }
    
    CLI();
    if(privacy != PRIV_DUMMY)
		 QueueAddItem(process, process->queue, &RunQueue); // Ready to run, add it to the list
	else {
		 process->state = TS_DUMMY; // Initing the processing
		 CurrentProcess = process; // Set as default process
	}
	STI();
    return ret; // PID
}
// Give up CPU time
void ProcessYield()
{
	/*******************************
	********************************
	-> MY GOD, WHAT A SHIT HA ??? <-
	********************************
    
    Calling this function is very bad for the system:
            - It will mess up with the blocked times;
            - A ProcessSleep() of 10 cycles will last less thasn usual if a process
            is always calling this function.
	
	Although this, there are advantages:
            - If a process is just looping CPU time will be wasted. BUT if the process
            call this function every time it has no job, the CPU time will be used.
            - This will result in an improvement of perfomance.
    */	
    ProcessStruct *proc = GetCurrentProcess();
	// Set current process time = 0,so the schedule will change to another task
	CLI();
	if(proc != NULL)
		 proc->tcurrent->time=0;
    // re-schedule int
    asm("int $0x31");
    
}
ProcessStruct *GetProcess(unsigned long pid)
{	
    QueueStruct *q = RunQueue;
	ProcessStruct *p;
    
    if(CurrentProcess->id == pid)
         return CurrentProcess;
	
    // RunQueue
    while(q != NULL) {
		 p = (ProcessStruct *)q->data;
		 if(p != NULL)
			  if(p->id == pid)
                   return p;
		 q = q->next;
	}
    // WaitQueue
	q = WaitQueue;
	while(q != NULL) {
		 p = (ProcessStruct *)q->data;
		 if(p != NULL)
			  if(p->id == pid)
                   return p;
		 q = q->next;
	}
    return NULL;
}
void ProcessExit(int result)
{
	ProcessStruct *p = GetCurrentProcess();
	if(p == NULL)
		 return; // Cant exit the only process

    CLI();
	DM("process %ld exited with error=%d", p->id, result);
	p->state = TS_ZOMBIE;
	p->tcurrent->time = 0;
	p->tcurrent->state = TS_ZOMBIE;
    while(1) {
         QueueAddItem(p, p->queue, &ZombieQueue);
	     ProcessYield();
    }
}
int ProcessKill(unsigned long pid)
{
    ProcessStruct *p = GetProcess(pid);
    if(p == NULL)
         return -ESRCH;
    
    if(p == GetCurrentProcess())
         ProcessExit(ESUCCESS);

    // ...
    return ESUCCESS;
}     
int ProcessSleep(unsigned long timeout)
{
	ProcessStruct *p = GetCurrentProcess();
	if(p == NULL)
		 return -EAGAIN;

    CLI();
	DM("pid:%ld('%s') - %ld cycles", p->id, p->name, timeout);
    p->timeout = timeout;
	p->state = TS_BLOCKED;
	QueueAddItem(p, p->queue, &WaitQueue);
	STI();
	
	ProcessYield(); // Change to another process
	return ESUCCESS; // return here when process wakes
}
int ProcessWake(ProcessStruct *p)
{
    if(p == NULL)
		 return -EINVAL;
	
	CLI();
	DM("pid:%ld('%s')", p->id, p->name);
    p->state = TS_READY;
	QueueAddItem(p, p->queue, &RunQueue);
	STI();
    
    return ESUCCESS;
}
int ProcessWait(unsigned long pid)
{
    if(!pid)
		 return -EINVAL;

    DM("waiting pid:%ld", pid); // Wait for the requested PID to terminate
    while(1) {
		 if(GetProcess(pid) == NULL)
			  break;
		 ProcessYield(); // Process is still active
	}
	DM("pid:%ld exited", pid);
	return ESUCCESS;
}
int ProcessRename(char *new_name)
{
    // This function is only used by cosmetic reason's
    ProcessStruct *p = GetCurrentProcess();
    
	DM("renaming process '%s' to '%s'", p->name, new_name);
    strncpy(p->name, new_name, TASK_NAME_SIZE); // Process Name
    return ESUCCESS;
}
/** @fn static int ZombieSlayer(int argc, char **argv)
    @brief Zombie Process Handler. Takes care of dead processes
    @param argc Argument count
    @param argv Arguments
    @ingroup sys_kernel_process
    @return Error Code
    @internal
*/
static int ZombieSlayer(int argc, char **argv)
{
	ProcessStruct *p;
	QueueStruct *q;
	ThreadStruct *t;
	
    while(1) {
         if((ZombieQueue != NULL) && (ZombieQueue->data != NULL)) { // Then all processes
              p = ZombieQueue->data;
              if(p->state != TS_ZOMBIE) {
                   DM("Living Process in Zombie Queue inserting back into RunQueue (pid=%ld)", p->id);
                   ZombieQueue = ZombieQueue->next;
                   QueueAddItem(p, p->queue, &RunQueue);
                   continue;
              }
              ZombieQueue->data = NULL;
              if(ZombieQueue->next != NULL)
                   ZombieQueue = ZombieQueue->next;
              else 
			       ZombieQueue = NULL;
			       
              CLI();
              DM("Destroying zombie process! (pid=%ld('%s'))", p->id, p->name);
              DM("  utime: %d.%03dms, stime: %d.%03dms", (int)p->utime/1000, (int)p->utime % 1000, (int)p->stime / 1000, (int)p->stime % 1000);
              
              while(p->nthreads != 0) {
                   // No special needs for removing threads because we are killing the whole process
                   q = p->threads;
                   while(q != NULL) {
                        t = (ThreadStruct *)q->data;
                        if(t != NULL) { // Found a valid thread
                             DM("Destroying zombie thread! (id=%ld/pid=%ld('%s'))", t->id, p->id, p->name);
                             DM("  utime: %d.%03dms, stime: %d.%03dms", (int)t->utime/1000, (int)t->utime % 1000, (int)t->stime / 1000, (int)t->stime % 1000);
                             x86ThreadArchDestroy(&t->ArchThread, t->privacy);
                             kfree(t->queue);
                             kfree(t);
                             p->nthreads--;
                        }
                        if(q->next == NULL)
                             break; 
                        q = q->next;
                   }
              }
              x86ProcessArchDestroy(&p->ArchProcess);
              kfree(p->queue);
		      kfree(p);
              DM("Process destroyed");
              STI();
         } else
              ProcessYield();
	}
	return ESUCCESS;
}
/** @fn static ProcessStruct *GetNextRunnable()
    @brief Get next Runnable process
    @ingroup sys_kernel_process 
    @return Next runnable process struct pointer
    @internal
*/
static ProcessStruct *GetNextRunnable()
{
	QueueStruct **q;
	QueueStruct *tmp;
	q = &RunQueue;
	ProcessStruct *p;
	ProcessStruct *result;

	if(q == NULL || *q == NULL || (*q)->data == NULL)
		 return CurrentProcess; // No more processes to run

	// Get the first process in the queue
	result = p = (ProcessStruct *)(*q)->data;
	
	if((*q)->next != NULL) {
		 tmp = *q;
		 (*q) = (*q)->next;
		 tmp = NULL;
		 (*q)->prev = NULL;
	} else
		 *q = NULL;
	p->queue->prev = p->queue->next = NULL;
	return result;
}
/** @fn static int ThreadGetNextRunnable(ProcessStruct *p)
    @brief Checks if there are threads left to run in process p
    
    The threads are in a linked list. This function checks if all threads of process threads list have run.
    If there are still threads to run the function redefines p->tcurrent and returns TRUE.
    If we are at the end of the list return FALSE so that the Scheduler changes to another process.
    Also defines p->tcurrent to point to process threads list begin.
    If there is only one thread running this function will return always FALSE.
    If the current thread is in Zombie state redefines the p->tcurrent to a valid thread and returns FALSE.
    @ingroup sys_kernel_process 
    @param p Process
    @return TRUE or FALSE
    @internal
*/
static int ThreadGetNextRunnable(ProcessStruct *p)
{
	// Procurar na lista pelo thread em execução
	// Verificar se existem ainda mais para executar
	// N: Define o inicio da lista como tcurrent e retorna 0
	// S: Define esse thread como o tcurrent e retorna 1
    QueueStruct **q;
    q = &p->threads;
    
    if(q == NULL || *q == NULL || (*q)->data == NULL)
         return 0; // No threads to run
    
    if(p->tcurrent->state == TS_ZOMBIE || p->state == TS_ZOMBIE) {
         p->tcurrent = (ThreadStruct *)(*q)->data;
         return 0;
    }
    if(p->tcurrent->queue->next != NULL) { // There are threads to run. Return the next one
         p->tcurrent = (ThreadStruct *)p->tcurrent->queue->next->data;
         return 1;
    } else if(p->tcurrent->queue->prev != NULL) { // We reached the tail, return to begin    
         p->tcurrent = (ThreadStruct *)(*q)->data; 
         return 0; // But change to a new process
    } else // There is only one thread to run
         return 0;
}
/** @fn ProcessStruct * Schedule()
    @brief Schedule function of process manager
    
    This function is called by ThreadArchSchedule(...) and must return either a NULL pointer or
    a ProcessStruct pointer to the next task to run. The process struct has a pointer to the current
    thread running that is updated using ThreadGetNextRunnable(...).
    It's important to return using one of this 2 methods. The ThreadArchSchedule(...) will pick up
    the process thread stack and write it into CPU registers then iret to the task.
    Strange things can happen if the Schedule() function is not called by ThreadArchSchedule(...) so don't
    mess up with this.
    @ingroup sys_kernel_process 
    @return Next runnable process pointer
*/
ProcessStruct * Schedule()
{
    ProcessStruct *runnable;
	ProcessStruct *p;
	QueueStruct *q;
	int ret = 0;
	
	// Decrement wait queue time
    q = WaitQueue;
	while(q != NULL) {
		 if(q->data == NULL) {
			  q = q->next;
			  continue;
		 }
		 p = (ProcessStruct *)q->data;
		 p->timeout--;
		
         if(p->timeout == 0) {
			  // if timeout, remove from wait queue = RemoveProcessQueue()
			  if(p->queue->prev != NULL)
				   p->queue->prev->next = p->queue->next;
			  if(p->queue->next != NULL)
				   p->queue->next->prev = p->queue->prev;
				   
			  // The process is in the queue top
              if(WaitQueue->data == (void *)p) {
				   if(WaitQueue->next == NULL)
					    WaitQueue = NULL;
				   else {
					    WaitQueue = WaitQueue->next;
					    WaitQueue->prev = NULL;
				   }
			  }
			  p->queue->next = p->queue->prev = NULL;	
			  ProcessWake(p);
		 }
		 q = q->next;
	}
	
	p = GetCurrentProcess();
    runnable = NULL;

    // Still time to run
    if((p->tcurrent->time != 0) && (p->state == TS_RUNNING) && (p->tcurrent->state == TS_READY)) {
         p->tcurrent->time--;
         return NULL;
    }
       
    // Get new thread or process
    if(p->state == TS_RUNNING) {
         if(p->tcurrent->time == 0) // Prepare for next round
              p->tcurrent->time = 10;
         while(1) {
              // First get next runnable thread
              ret = ThreadGetNextRunnable(p);
              if(ret) { // We still have threads in process to run
                   if(p->tcurrent->state == TS_READY) { // What about thread state ? /** @todo here */
                        runnable = p;
                        break; // Thread is ready, run it
                   }
              } else
                   break;
         }
    }

    if(runnable == NULL) {
         //
         // IF WE REACH HERE WE HAVE TO CHANGE TO ANOTHER PROCESS
         //
         runnable = p;
         do {
              // Anyway check if the process status is valid
              if(runnable->state == TS_READY) {
                   if((runnable->tcurrent->state == TS_READY) && (runnable->tcurrent->time))
                        break;
              } else if(runnable->state == TS_BLOCKED) {
			       QueueAddItem(runnable, runnable->queue, &WaitQueue);
			       DM("Adding pid=%ld to WaitQueue", runnable->id);
              }	else if(runnable->state == TS_ZOMBIE) {
			       QueueAddItem(runnable, runnable->queue, &ZombieQueue);
			       DM("Adding pid=%ld to ZombieQueue", runnable->id);
              } else if(runnable->state == TS_RUNNING) {
                   // Prepare for next round
                   p->state = TS_READY;
		           QueueAddItem(p, p->queue, &RunQueue);
              }
              runnable = GetNextRunnable(); // Get Next Process
         } while(1);
    }
	//DM("Switching to pid=%d, tid=%d", runnable->id, runnable->tcurrent->id);
    // set our next process as running
	runnable->state = TS_RUNNING;
    return runnable;
}
int ProcessingInit()
{
    NextPID = 0;
    	
	/** @note Este processo serve para inicializar o gestor de processos */
    ProcessCreate("NullProcess", NULL, NORMAL_PRIO, DummyThread, PRIV_DUMMY);
    ProcessCreate("ZombSlayer", NULL, NORMAL_PRIO, ZombieSlayer, PRIV_KERNEL);
    x86IdtSetGate(49, (unsigned)x86Irq17AsmHandler, 0x08, 0x8E);
	DM("------- Processing inited -------");
    return ESUCCESS;
}
