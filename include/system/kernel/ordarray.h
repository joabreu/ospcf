/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\ordarray.h
    @brief Ordered array manipulation
*/
#ifndef INCLUDE_SYSTEM_KERNEL_ORDARRAY_H
#define INCLUDE_SYSTEM_KERNEL_ORDARRAY_H

/*
   This array is insertion sorted - it always remains in a sorted state (between calls).
   It can store anything that can be cast to a void* -- so a u32int, or any pointer.
*/
/** @brief Used to store anything that can be cast to a void* */
typedef void* type_t;
/** @brief  Structure of a general LessThanType function. This function should return nonzero if the first argument is less than the second. Else it should return zero */
typedef char (*LessThanType)(type_t,type_t);

/** @brief Structure of an ordered array */
typedef struct
{
   	/** @brief Ordered array pointer */
	type_t *array;
	/** @brief Ordered array size */
   	unsigned int size;
   	/** @brief Orderred array maximum size */
	unsigned int max_size;
   	/** @brief LessThan function */
	LessThanType LessThan;
} OrderedArrayStruct;

/** @fn char StandardLessThan(type_t a, type_t b)
    @brief Checks if a<b
    @ingroup sys_kernel
    @param a Something
    @param b Something
    @return TRUE if a<b, FALSE otherwise
*/
char StandardLessThan(type_t a, type_t b);
/** @fn OrderedArrayStruct OrderedArrayCreate(unsigned int max_size, LessThanType less_than)
    @brief Create an ordered array
    @ingroup sys_kernel
    @param max_size Array maximum size
    @param less_than Aux function, your personal StandardLessThan()
    @return New array
*/
OrderedArrayStruct OrderedArrayCreate(unsigned int max_size, LessThanType less_than);
/** @fn OrderedArrayStruct OrderedArrayPlace(void *addr, unsigned int max_size, LessThanType less_than)
    @brief Create an ordered array, place it in addr
    @ingroup sys_kernel
    @param addr Place where array begin is
    @param max_size Array maximum size
    @param less_than Aux function, your personal StandardLessThan()
    @return New array
*/
OrderedArrayStruct OrderedArrayPlace(void *addr, unsigned int max_size, LessThanType less_than);
/** @fn void OrderedArrayInsert(type_t item, OrderedArrayStruct *array)
    @brief Insert an item into an array
    @ingroup sys_kernel
    @param item Item to insert
    @param array Array to insert in
*/
void OrderedArrayInsert(type_t item, OrderedArrayStruct *array);
/** @fn type_t OrderedArrayLookup(unsigned int i, OrderedArrayStruct *array)
    @brief Return item in position i of array
    @ingroup sys_kernel
    @param i Item position
    @param array Array
    @return Item in i position
*/
type_t OrderedArrayLookup(unsigned int i, OrderedArrayStruct *array);
/** @fn void OrderedArrayRemove(unsigned int i, OrderedArrayStruct *array)
    @brief Remove item in position i of array
    @ingroup sys_kernel
    @param i Item position
    @param array Array
*/
void OrderedArrayRemove(unsigned int i, OrderedArrayStruct *array);

#endif
