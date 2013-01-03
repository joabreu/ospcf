/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\queue.h
    @brief Queue management functions
*/
#ifndef INCLUDE_SYSTEM_KERNEL_QUEUE_H
#define INCLUDE_SYSTEM_KERNEL_QUEUE_H

/** @brief Queue struct */
typedef struct _QueueStruct
{
	/** @brief Next item */
	struct _QueueStruct *next;
	/** @brief Previous item */
	struct _QueueStruct *prev;
	/** @brief Item */
	void *data;
} QueueStruct;

/** @fn int QueueAddItem(void *item, QueueStruct *itemqueue, QueueStruct **q)
    @brief Add an item to the queue q
    @ingroup sys_kernel 
    @param item Item to add
    @param itemqueue This item needs to have an internal queue that will point to queue q
    @param q Add the item to this queue
    @return Error Code
*/
int QueueAddItem(void *item, QueueStruct *itemqueue, QueueStruct **q);
/** @fn int QueueRemoveItem(QueueStruct *itemqueue)
    @brief Remove an item from its queue
    @ingroup sys_kernel 
    @param itemqueue Item internal queue
    @return Error Code
*/
int QueueRemoveItem(QueueStruct *itemqueue);

#endif
