/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\ordarray.c
    @brief Ordered arrays manipulation
*/
/************************ PORTED TO PCF ************************/
// ordered_array.c -- Implementation for creating, inserting and deleting
//                    from ordered arrays.
//                    Written for JamesM's kernel development tutorials.
#include <string.h>
#include <kernel\debug.h>
#include <kernel\ordarray.h>
#include <kernel\mem\kheap.h>

char StandardLessThan(type_t a, type_t b)
{
    return (a<b)?1:0;
}
OrderedArrayStruct OrderedArrayCreate(unsigned int max_size, LessThanType less_than)
{
    OrderedArrayStruct to_ret;
    to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.LessThan = less_than;
    return to_ret;
}
OrderedArrayStruct OrderedArrayPlace(void *addr, unsigned int max_size, LessThanType less_than)
{
    OrderedArrayStruct to_ret;
    to_ret.array = (type_t*)addr;
    memset(to_ret.array, 0, max_size*sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.LessThan = less_than;
    return to_ret;
}
void OrderedArrayInsert(type_t item, OrderedArrayStruct *array)
{
    KASSERT(array->LessThan);
    unsigned int iterator = 0;
    while (iterator < array->size && array->LessThan(array->array[iterator], item))
         iterator++;
    if (iterator == array->size) // just add at the end of the array.
         array->array[array->size++] = item;
    else
    {
         type_t tmp = array->array[iterator];
         array->array[iterator] = item;
         while (iterator < array->size)
         {
              iterator++;
              type_t tmp2 = array->array[iterator];
              array->array[iterator] = tmp;
              tmp = tmp2;
         }
         array->size++;
    }
}
type_t OrderedArrayLookup(unsigned int i, OrderedArrayStruct *array)
{
    KASSERT(i < array->size);
    return array->array[i];
}
void OrderedArrayRemove(unsigned int i, OrderedArrayStruct *array)
{
    while (i < array->size)
    {
         array->array[i] = array->array[i+1];
         i++;
    }
    array->size--;
}
