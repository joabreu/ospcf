/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\queue.c
    @brief Queue management functions
*/
#include <errno.h>
#include <string.h>
#include <kernel\queue.h>
#include <kernel\mem\kheap.h>

int QueueAddItem(void *item, QueueStruct *itemqueue, QueueStruct **q)
{
	QueueStruct *tail;
	QueueStruct *i;

	if((*q) == NULL) // Check if queue is empty
	{
		 // A inserir dados numa estrutura nova
		 // A estrutura já deve ter sido alocada mas vamos verificar
		 if(itemqueue  == NULL)
		 {
			  itemqueue = (QueueStruct *)kmalloc(sizeof(QueueStruct));
			  if(itemqueue == NULL)
				   return -ENOMEM;
			  memset(itemqueue, 0, sizeof(QueueStruct));
		 }
		 (*q) = itemqueue;
		 (*q)->next = NULL;
		 (*q)->prev = NULL;
		 (*q)->data = item;
		 return ESUCCESS;
	}
	// Encontrar a cauda da fila e adiconar o processo lá
	tail = (*q);
	while(tail->next != NULL)
		 tail = tail->next;
	//  once again this should already be allocated when the process was
	// created, but just in case its not, we'll allocate a queue struct
	if(itemqueue == NULL)
	{
		 itemqueue = (QueueStruct *)kmalloc(sizeof(QueueStruct));
         if(itemqueue == NULL)
			  return -ENOMEM;
		 memset(itemqueue, 0, sizeof(QueueStruct));
	}
	//this process now becomes the tail of the queue
	tail->next = itemqueue;
	itemqueue->prev = tail;
	tail = tail->next;
	tail->next = NULL;
	tail->data = item;
	return ESUCCESS;
}
int QueueRemoveItem(QueueStruct *itemqueue)
{
	if(itemqueue->prev != NULL)
		 itemqueue->prev->next = itemqueue->next;
	if(itemqueue->next != NULL)
		 itemqueue->next->prev = itemqueue->prev;
	return ESUCCESS;
}
