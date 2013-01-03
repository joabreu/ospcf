/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\process\debug.c
    @brief Process manager debug functions
*/
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <kernel\proc.h>
#include <kernel\console.h>
#include <kernel\queue.h>

void ProcessPrintInfo(ProcessStruct *p)
{
    char state[10];
    char tstate[10];
    char priv[10];
    char prio[10];
    
    if(p == NULL)
         return;
         
    switch(p->state) {
		 case TS_READY:    sprintf(state, "Ready");break;
	     case TS_RUNNING:  sprintf(state, "Running");break;
         case TS_BLOCKED:  sprintf(state, "Blocked");break;
         case TS_ZOMBIE:   sprintf(state, "Zombie");break;
         default:          sprintf(state, "Unknown");break;
    }
    switch(p->tcurrent->state) {
		 case TS_READY:    sprintf(tstate, "Ready");break;
	     case TS_RUNNING:  sprintf(tstate, "Running");break;
         case TS_BLOCKED:  sprintf(tstate, "Blocked");break;
         case TS_ZOMBIE:   sprintf(tstate, "Zombie");break;
         default:          sprintf(tstate, "Unknown");break;
    }
    switch(p->privacy) {
         case PRIV_KERNEL: sprintf(priv, "Kernel");break;
         case PRIV_USER:   sprintf(priv, "User");break;
         case PRIV_DUMMY:  sprintf(priv, "Dummy");break;
         default:          sprintf(priv, "Unknown");break;
    }
    switch(p->priority) {
         case IDLE_PRIO:   sprintf(prio, "Idle");break;
         case LOW_PRIO:    sprintf(prio, "Low");break;
         case NORMAL_PRIO: sprintf(prio, "Default");break;
         case HIGH_PRIO:   sprintf(prio, "High");break;
         default:          sprintf(prio, "Unknown");break;
    }
    kprintf("Process '%s':\n", p->name);
    kprintf("pid=%d\tstate=%d(%s)\tprivacy=%d(%s)\n", p->id, p->state, state, p->privacy, priv);
    kprintf("priority=%d(%s)\ttimeout=%d\tnthreads=%d\n", p->priority, prio, p->timeout, p->nthreads);
    kprintf("utime=%d\tstime=%d\n", p->utime, p->stime);
    kprintf("page_directory=0x%x\n", p->ArchProcess.PageDirectory->PhysicalAddr);
// replace to ThreadPrintInfo()
    kprintf(" --> Current Running Thread:\n");
    kprintf("   tid=%d\tstate=%d(%s)\n", p->tcurrent->id, p->tcurrent->state, tstate);
    kprintf("   entry=0x%x\n", p->tcurrent->entry);
    kprintf("   time=%d\ttimeout=%d\n", p->tcurrent->time, p->tcurrent->timeout);
    kprintf("   utime=%d\tstime=%d\n", p->tcurrent->utime, p->tcurrent->stime);
// Arch dependent Settings
    kprintf("   stack_base=0x%x\tkernel_stack_base=0x%x\n",
    p->tcurrent->ArchThread.stack_base, p->tcurrent->ArchThread.kernel_stack_base);
    kprintf("   stack=0x%x\tkernel_stack=0x%x\n",
    p->tcurrent->ArchThread.stack, p->tcurrent->ArchThread.kernel_stack);
}
/** @fn static void ProcessPrintHelper(ProcessStruct *p)
    @brief Print basic info about a single process
    @ingroup sys_kernel_process
    @param p Process
    @internal
*/
static void ProcessPrintHelper(ProcessStruct *p)
{
    char state[10];
    
    if(p == NULL)
         return;
         
    switch(p->state) {
		 case TS_READY:    sprintf(state, "Ready");break;
	     case TS_RUNNING:  sprintf(state, "Running");break;
         case TS_BLOCKED:  sprintf(state, "Blocked");break;
         case TS_ZOMBIE:   sprintf(state, "Zombie");break;
         default:          sprintf(state, "Unknown");break;
    }
    kprintf("%s", p->name);
    kprintf("\r\t\t\t %d", (int)p->id);
    kprintf("\r\t\t\t\t %s", state);
    kprintf("\r\t\t\t\t\t %d", (int)p->priority);
    kprintf("\r\t\t\t\t\t\t\t %d\n", (int)p->nthreads);
}
/** @fn static void ListQueue(QueueStruct *queue)
    @brief List all processes in a queue
    @ingroup sys_kernel_process 
    @param queue Queue
    @internal
*/
static void ListQueue(QueueStruct *queue)
{
    QueueStruct *q = queue;
    ProcessStruct *p;
    
    if(q == NULL || q->data == NULL) {
        kprintf("Empty Queue\n");
        return;
    }
    while(q != NULL) {
         p = (ProcessStruct *)q->data;
         if(p != NULL)
              ProcessPrintHelper(p);
         q = q->next;
    }   
}
extern QueueStruct *RunQueue;
extern QueueStruct *WaitQueue;
extern QueueStruct *ZombieQueue;
void DumpProcesses()
{
	ProcessStruct *p; // Running process pointer
	
    kprintf("Listing all processes...\n");
   	
    kprintf("Process\t\t\t Pid\t State\t Priority\t Threads\n");
	kprintf("-------\t\t\t ---\t -----\t --------\t -------\n");
    
    kprintf("-- Run Queue --\n");
	ListQueue(RunQueue);
	kprintf("-- Wait Queue --\n");
	ListQueue(WaitQueue);
	kprintf("-- Zombie Queue --\n");
	ListQueue(ZombieQueue);
    kprintf("-- Current Process --\n");
    p = GetCurrentProcess();
    ProcessPrintHelper(p);
}
