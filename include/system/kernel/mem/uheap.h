/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\mem\uheap.h
    @brief User heap manager
*/
#ifndef INCLUDE_SYSTEM_KERNEL_MEM_UHEAP_H
#define INCLUDE_SYSTEM_KERNEL_MEM_UHEAP_H

/** @brief User Heap Start Address */
#define UHEAP_START         0xE0000000 // 3,5GB
/** @brief User Heap Initial Size */
#define UHEAP_INITIAL_SIZE  0x100000   // 1MB
/** @brief User Heap Maximum Size */
#define UHEAP_MAX_SIZE      0xEFFFFFFF // 3,9999999 GB

/** @fn unsigned int umalloc_int(unsigned int sz, int align, unsigned int *phys)
    @brief Alloc sz bytes using user heap
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @param align Return a page-aligned address if true
    @param phys Store physical address of block in here
    @return Address
*/
unsigned int umalloc_int(unsigned int sz, int align, unsigned int *phys);
/** @fn void ufree(void *p)
    @brief Frees previous allocated address
    @ingroup sys_kernel_mem
    @param p Address
*/
void ufree(void *p);
/** @fn unsigned int umalloc_a(unsigned int sz)
    @brief Alloc sz bytes using user heap. Return a page-aligned address
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @return Address
*/
unsigned int umalloc_a(unsigned int sz);
/** @fn unsigned int umalloc_p(unsigned int sz, unsigned int *phys)
    @brief Alloc sz bytes using user heap. Store physical location of address in phys
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @param phys Store physical address of block in here
    @return Address
*/
unsigned int umalloc_p(unsigned int sz, unsigned int *phys);
/** @fn unsigned int umalloc_ap(unsigned int sz, unsigned int *phys)
    @brief Alloc sz bytes using user heap. Return a page-aligned address and store its location in phys
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @param phys Store physical address of block in here
    @return Address
*/
unsigned int umalloc_ap(unsigned int sz, unsigned int *phys);
/** @fn unsigned int umalloc(unsigned int sz)
    @brief Alloc sz bytes using user heap
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @return Address
*/
unsigned int umalloc(unsigned int sz);

#endif
