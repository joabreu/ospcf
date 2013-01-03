/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\memory\uheap.c
    @brief User Heap memory management
*/
#include <kernel\mem\heap.h>
#include <kernel\x86\mem.h>
#include <kernel\x86\paging.h>

extern x86PageDirectoryStruct *KernelDirectory;
/** @brief User Heap Structure */
HeapStruct *UserHeap = 0;

unsigned int umalloc_int(unsigned int sz, int align, unsigned int *phys)
{
    if(UserHeap != 0) {
         /* Allocate memory for the user
          * using the kernel page directory
          * User Heap must be created
          */
         void *addr = alloc(sz, (unsigned char)align, UserHeap);
         if(phys != 0) {
              x86PageStruct *page = x86PagingGetPage((unsigned int)addr, 0, KernelDirectory);
              *phys = page->frame*PAGE_SIZE + ((unsigned int)addr&0xFFF);
         }
         return (unsigned int)addr;
    }
    else
         return 0; // EINITFIRST
}
void ufree(void *p)
{
    free(p, UserHeap);
}
unsigned int umalloc_a(unsigned int sz)
{
    return umalloc_int(sz, 1, 0);
}
unsigned int umalloc_p(unsigned int sz, unsigned int *phys)
{
    return umalloc_int(sz, 0, phys);
}
unsigned int umalloc_ap(unsigned int sz, unsigned int *phys)
{
    return umalloc_int(sz, 1, phys);
}
unsigned int umalloc(unsigned int sz)
{
    return umalloc_int(sz, 0, 0);
}
