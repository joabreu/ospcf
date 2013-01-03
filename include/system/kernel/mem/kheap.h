/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\mem\kheap.h
    @brief Kernel heap manager
*/
#ifndef INCLUDE_SYSTEM_KERNEL_MEM_KHEAP_H
#define INCLUDE_SYSTEM_KERNEL_MEM_KHEAP_H

/** @brief Kernel Heap Start Address */
#define KHEAP_START         0xC0000000 // 3GB
/** @brief Kernel Heap Initial Size */
#define KHEAP_INITIAL_SIZE  0x100000   // 1MB
/** @brief Kernel Heap Maximum Size */
#define KHEAP_MAX_SIZE      0xCFFFFFFF // 3,499999 GB

/** @fn unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys)
    @brief Alloc sz bytes using kernel heap
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @param align Return a page-aligned address if true
    @param phys Store physical address of block in here
    @return Address
*/
unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys);
/** @fn void kfree(void *p)
    @brief Frees previous allocated address
    @ingroup sys_kernel_mem
    @param p Address
*/
void kfree(void *p);
/** @fn unsigned int kmalloc_a(unsigned int sz)
    @brief Alloc sz bytes using kernel heap. Return a page-aligned address
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @return Address
*/
unsigned int kmalloc_a(unsigned int sz);
/** @fn unsigned int kmalloc_p(unsigned int sz, unsigned int *phys)
    @brief Alloc sz bytes using kernel heap. Store physical location of address in phys
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @param phys Store physical address of block in here
    @return Address
*/
unsigned int kmalloc_p(unsigned int sz, unsigned int *phys);
/** @fn unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys)
    @brief Alloc sz bytes using kernel heap. Return a page-aligned address and store its location in phys
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @param phys Store physical address of block in here
    @return Address
*/
unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys);
/** @fn unsigned int kmalloc(unsigned int sz)
    @brief Alloc sz bytes using kernel heap
    @ingroup sys_kernel_mem
    @param sz Bytes to alloc
    @return Address
*/
unsigned int kmalloc(unsigned int sz);

#endif
