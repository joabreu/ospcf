/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\memory\kheap.c
    @brief Kernel Heap memory management
*/
/************************ PORTED TO PCF ************************/
// kheap.c -- Kernel heap functions, also provides
//            a placement malloc() for use before the heap is 
//            initialised.
//            Written for JamesM's kernel development tutorials.
#include <kernel\mem\heap.h>
#include <kernel\x86\mem.h>
#include <kernel\x86\paging.h>

/** @brief Kernel physical end. Defined during compilation. */
extern unsigned int end;
/** @brief For intial allocations

    -- Some little history about Placement Adress : --
    This Variable is ***VERY VERY*** Important. In the begginig it points to the kernel physical end, but when we load modules or alloc memory (this memory can't be freed) the address will increase.
    This variable become unused when we enable paging, but before that is very important to avoid messing up with memory reserved to the kernel.
*/
unsigned int PlacementAddress = (unsigned int)&end;
extern x86PageDirectoryStruct *KernelDirectory;
/** @brief Kernel Heap Structure */
HeapStruct *KernelHeap = 0;

unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys)
{
    if(KernelHeap != 0) {
         /* Allocate memory for the kernel
          * using the kernel page directory
          * Kernel Heap must be created
          */
         void *addr = alloc(sz, (unsigned char)align, KernelHeap);
         if(phys != 0) {
              x86PageStruct *page = x86PagingGetPage((unsigned int)addr, 0, KernelDirectory);
              *phys = page->frame*PAGE_SIZE + ((unsigned int)addr&0xFFF);
         }
         return (unsigned int)addr;
    } else {
         /* Allocate memory that can't be fread
          * by just imcrementing an address. Used
          * before paging initialization
          */
         if((align == 1) && (PlacementAddress & 0xFFFFF000/* 0x1000 + 0xFFFFF000 = 0*/)) {
              // Align the placement address;
              PlacementAddress &= 0xFFFFF000;
              PlacementAddress += PAGE_SIZE;
         }
         if (phys) {
              *phys = PlacementAddress;
         }
         unsigned int tmp = PlacementAddress;
         PlacementAddress += sz;
         return tmp;
    }
}
void kfree(void *p)
{
    free(p, KernelHeap);
}
unsigned int kmalloc_a(unsigned int sz)
{
    return kmalloc_int(sz, 1, 0);
}
unsigned int kmalloc_p(unsigned int sz, unsigned int *phys)
{
    return kmalloc_int(sz, 0, phys);
}
unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys)
{
    return kmalloc_int(sz, 1, phys);
}
unsigned int kmalloc(unsigned int sz)
{
    return kmalloc_int(sz, 0, 0);
}
