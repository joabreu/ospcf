/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\process\thread.c
    @brief Thread management
*/
#include <errno.h>
#include <string.h>
#include <kernel\debug.h>
#include <kernel\proc.h>
#include <kernel\queue.h>
#include <kernel\mem\kheap.h>
#include <kernel\x86\task.h>
#include <kernel\x86\cpu.h>

/** @brief If defined debug will be outputed */
#define DEBUG_THRD
#ifdef DEBUG_THRD
  /** @brief If debug is selected print a message using the standard DEBUG(...) function */
  #define DM(Msg...) DEBUG(Msg)
#else
  /** @brief Debug is not selected */ 
  #define DM(Msg...) {}
#endif

/** @brief Next valid Thread ID */
static unsigned long NextTID;

/** @fn static unsigned long ThreadInit(ThreadStruct *t, ProcessStruct *p, void *entry, unsigned long *argv, unsigned char privacy)
    @brief Initializate a thread. Called from ThreadCreate(...)
    @ingroup sys_kernel_process 
    @param t New thread Struct
    @param p Process that this thread belong
    @param entry Thread entry function
    @param argv Arguments to suply the thread
    @param privacy Thread privileges (kernel/user)
    @return New thread ID or Error Code
    @internal
*/
static unsigned long ThreadInit(ThreadStruct *t, ProcessStruct *p, void *entry, unsigned long *argv, unsigned char privacy)
{
    int ret = 0;
    
    t->id = NextTID++;
    t->state = TS_READY;
    t->privacy = privacy;
    t->priority = p->priority;
    
    t->parent = p;
    t->entry = (unsigned long)entry;
    
    t->time = 10;
    t->timeout = 0;
    t->utime = 0;
    t->stime = 0;
    
    //t->CurrentDir = privacy == PRIV_USER ? p->tcurrent->CurrentDir : VfsGetRootVnode();
    t->queue = (QueueStruct *)kmalloc(sizeof(QueueStruct));
    if(t->queue == NULL)
         return -ENOMEM;
    memset(t->queue, 0, sizeof(QueueStruct));

    t->queue->next = 0;
    t->queue->prev = 0;
    t->queue->data = (void *)t;
    
    ret = x86ThreadArchInit(&t->ArchThread, entry, argv, privacy); // Init arch dependent settings
    if(ret != ESUCCESS) {
	     DM("Failed initing thread arch (errno:%d)", ret);
	     kfree(t->queue);
         return -EIFAIL;
    }
    if(privacy == PRIV_DUMMY) { // Initing multitasking
         t->state = TS_DUMMY;
         t->time = 0;
    }
    if(p->tcurrent == 0) // Initing a process
         p->tcurrent = t;
    if(p->tmain == 0)
         p->tmain = t;
    
    QueueAddItem(t, t->queue, &p->threads);
    p->nthreads++;
    
    DM("new thread created (at 0x%x):\n\
    tid=%ld | parent_id=%ld\n\
    entry=0x%x | queue=0x%x\n\
    kernel_stack_base=0x%x | stack_base=0x%x", t, t->id, t->parent->id, entry, t->queue,
    t->ArchThread.kernel_stack_base, t->ArchThread.stack_base);
    return t->id;
}
unsigned long ThreadCreate(ProcessStruct *process, void *entry, unsigned long *argv)
{
    ThreadStruct *thread;
    unsigned long ret = 0;
    
    thread = (ThreadStruct *)kmalloc(sizeof(ThreadStruct));
    if(thread == NULL)
         return -ENOMEM;
    memset(thread, 0, sizeof(ThreadStruct));
    
    ret = ThreadInit(thread, process, entry, argv, process->privacy);
    if(ret < 0) {
         kfree(thread);
         DM("Failed initing thread (errno:%ld)", ret);
         return -EIFAIL;
    }
    return ret; // TID
}
ThreadStruct *GetThread(ProcessStruct *p, unsigned long tid)
{
    QueueStruct *q;
    ThreadStruct *t;
    
    if(p == NULL)
         return NULL;
    
    q = p->threads;
    while(q != NULL) {
         t = (ThreadStruct *)q->data;
         if(t->id == tid)
              return t;
         if(q->next == NULL) 
              return NULL;
         q = q->next;
    }
    return NULL;
}
int ThreadKill(ThreadStruct *t)
{
    if(t == NULL)
         return -EINVAL;

    if(t->parent->tmain == t) // Killing main thread ??? nope
         return -EPERM;
         
    CLI();
	DM("killing thread %ld", t->id);
	t->state = TS_ZOMBIE;
	t->time = 0;
	STI();
	return ESUCCESS;
}
