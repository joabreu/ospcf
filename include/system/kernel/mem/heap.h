/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\mem\heap.h
    @brief Heap manager
*/
#ifndef INCLUDE_SYSTEM_KERNEL_MEM_HEAP_H
#define INCLUDE_SYSTEM_KERNEL_MEM_HEAP_H

#include <kernel\ordarray.h>

/** @brief Heap index size */
#define HEAP_INDEX_SIZE   0x20000
/** @brief Heap Magic for sanity checks */
#define HEAP_MAGIC        0x123890AB
/** @brief Heap minimum size */
#define HEAP_MIN_SIZE     0x70000

/** @brief Heap Header struct */
typedef struct
{
    /** @brief Magic number, used for error checking and identification */
    unsigned int magic;
    /** @brief 1 if this is a hole. 0 if this is a block */
    unsigned char is_hole;
    /** @brief Size of the block, including the end footer */
    unsigned int size;
} HeaderStruct;
/** @brief Heap Footer struct */
typedef struct
{
    /** @brief Magic number, same as in HeaderStruct, for sanity check */
    unsigned int magic;
    /** @brief Pointer to the block header */
    HeaderStruct *header;
} FooterStruct;
/** @brief Heap struct */
typedef struct
{
    /** @brief List of holes/blocks in this heap */
    OrderedArrayStruct index;
    /** @brief The start of our allocated space for this heap */
    unsigned int start_address;
    /** @brief The end of our allocated space for this heap. May be expanded up to max_address */
    unsigned int end_address;
    /** @brief The maximum address the heap can be expanded to */
    unsigned int max_address;
    /** @brief Should extra pages requested by us be mapped as supervisor-only? */
    unsigned int supervisor;
    /** @brief Should extra pages requested by us be mapped as read-only? */
    unsigned int readonly;
} HeapStruct;

/** @fn HeapStruct *CreateHeap(unsigned int start, unsigned int end_addr, unsigned int max, unsigned char supervisor, unsigned char readonly)
    @brief Create a new heap ( organized struct where we can save blocks, used by alloc )
    @ingroup sys_kernel_mem
    @param start Heap start address
    @param end_addr Heap end address
    @param max Heap maximum size
    @param supervisor If 1 heap is owned by kernel ( not accessible by user )
    @param readonly If 1 heap is readonly from user
    @return New heap struct
*/
HeapStruct *CreateHeap(unsigned int start, unsigned int end_addr, 
                       unsigned int max, unsigned char supervisor, unsigned char readonly);
/** @fn void *alloc(unsigned int size, unsigned char page_align, HeapStruct *heap)
    @brief Search for a size bytes block free in designated heap and mark them as used
    @ingroup sys_kernel_mem 
    @param size Block size
    @param page_align If 1 return a page aligned address
    @param heap Heap where to find block
    @return Pointer to free block
*/
void *alloc(unsigned int size, unsigned char page_align, HeapStruct *heap);
/** @fn void free(void *p, HeapStruct *heap)
    @brief Frees a previous allocated block
    @ingroup sys_kernel_mem 
    @param p Pointer to block
    @param heap Heap where this block is allocated
*/
void free(void *p, HeapStruct *heap);

#endif
