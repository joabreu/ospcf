/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\memory\heap.c
    @brief General Heap memory management
*/
#include <kernel\debug.h>
#include <kernel\ordarray.h>
#include <kernel\mem\heap.h>
#include <kernel\mem\kheap.h>
#include <kernel\x86\mem.h>
#include <kernel\x86\paging.h>

extern x86PageDirectoryStruct *KernelDirectory;

/** @fn static void expand(unsigned int NewSize, HeapStruct *Heap)
    @brief Expand a heap to a new size
    @ingroup sys_kernel_mem
    @param NewSize New heap size
    @param Heap Heap to expand
    @internal
*/
static void expand(unsigned int NewSize, HeapStruct *Heap)
{
    // Sanity check.
    KASSERT(NewSize > (Heap->end_address - Heap->start_address));

    // Get the nearest following page boundary.
    if((NewSize & 0xFFFFF000) != 0) {
         NewSize &= 0xFFFFF000;
         NewSize += PAGE_SIZE;
    }

    // Make sure we are not overreaching ourselves.
    KASSERT((Heap->start_address + NewSize) <= Heap->max_address);

    // This should always be on a page boundary.
    unsigned int old_size = Heap->end_address - Heap->start_address;

    unsigned int i = old_size;
    while(i < NewSize) {
         x86PagingAllocFrame(x86PagingGetPage( Heap->start_address+i, 1, KernelDirectory),
                     (Heap->supervisor)?1:0, (Heap->readonly)?0:1);
         i += PAGE_SIZE;
    }
    Heap->end_address = Heap->start_address+NewSize;
}
/** @fn static unsigned int contract(unsigned int NewSize, HeapStruct *Heap)
    @brief Contract a heap to a new size
    @ingroup sys_kernel_mem 
    @param NewSize New heap size
    @param Heap Heap to contract
    @return New heap size
    @internal
*/
static unsigned int contract(unsigned int NewSize, HeapStruct *Heap)
{
    // Sanity check.
    KASSERT(NewSize < (Heap->end_address - Heap->start_address));

    // Get the nearest following page boundary.
    if(NewSize&PAGE_SIZE) {
         NewSize &= PAGE_SIZE;
         NewSize += PAGE_SIZE;
    }

    // Don't contract too far!
    if(NewSize < HEAP_MIN_SIZE)
         NewSize = HEAP_MIN_SIZE;

    unsigned int old_size = Heap->end_address - Heap->start_address;
    unsigned int i = old_size - PAGE_SIZE;
    while(NewSize < i) {
         x86PagingFreeFrame(x86PagingGetPage(Heap->start_address+i, 0, KernelDirectory));
         i -= PAGE_SIZE;
    }
    Heap->end_address = Heap->start_address + NewSize;
    return NewSize;
}
/** @fn static int FindSmallestHole(unsigned int size, unsigned char PageAlign, HeapStruct *heap)
    @brief Find a smallest hole with at least size size
    @ingroup sys_kernel_mem 
    @param size Hole size
    @param PageAlign If >0 the function will find a hole page aligned
    @param heap Heap
    @return Hole position
    @internal
*/
static int FindSmallestHole(unsigned int size, unsigned char PageAlign, HeapStruct *heap)
{
    // Find the smallest hole that will fit.
    unsigned int iterator = 0;
    while(iterator < heap->index.size) {
         HeaderStruct *header = (HeaderStruct*)OrderedArrayLookup(iterator, &heap->index);
         // If the user has requested the memory be page-aligned
         if(PageAlign > 0) {
              // Page-align the starting point of this header.
              unsigned int location = (unsigned int)header;
              int offset = 0;
              if (((location+sizeof(HeaderStruct)) & 0xFFFFF000) != 0)
                   offset = PAGE_SIZE - (location+sizeof(HeaderStruct))%PAGE_SIZE;
              int hole_size = (int)header->size - offset;
              // Can we fit now?
              if(hole_size >= (int)size)
                   break;
         }
         else if(header->size >= size)
              break;
         iterator++;
    }
    // Why did the loop exit?
    if(iterator == heap->index.size)
         return -1; // We got to the end and didn't find anything.
    else
         return iterator;
}
/** @fn static char HeaderLessThan(void *a, void *b)
    @brief Admits a and b are headers struct pointers. Returns TRUE if a->size < b->size. FALSE otherwhise
    @ingroup sys_kernel_mem 
    @param a Header structure pointer
    @param b Header structure pointer
    @return TRUE or FALSE
    @internal
*/
static char HeaderLessThan(void *a, void *b)
{
    return (((HeaderStruct*)a)->size < ((HeaderStruct*)b)->size)?1:0;
}
HeapStruct *CreateHeap(unsigned int start, unsigned int end_addr, 
                       unsigned int max, unsigned char supervisor, unsigned char readonly)
{
    HeapStruct *heap = (HeapStruct*)kmalloc(sizeof(HeapStruct));

    // All our assumptions are made on startAddress and endAddress being page-aligned.
    KASSERT(start%PAGE_SIZE == 0);
    KASSERT(end_addr%PAGE_SIZE == 0);
    
    // Initialise the index.
    heap->index = OrderedArrayPlace((void*)start, HEAP_INDEX_SIZE, &HeaderLessThan);
    // Shift the start address forward to resemble where we can start putting data.
    start += sizeof(type_t)*HEAP_INDEX_SIZE;

    // Make sure the start address is page-aligned.
    if ((start & 0xFFFFF000) != 0) {
         start &= 0xFFFFF000;
         start += PAGE_SIZE;
    }
    // Write the start, end and max addresses into the heap structure.
    heap->start_address = start;
    heap->end_address = end_addr;
    heap->max_address = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    // We start off with one large hole in the index.
    HeaderStruct *hole = (HeaderStruct *)start;
    hole->size = end_addr-start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    OrderedArrayInsert((void*)hole, &heap->index);
    return heap;
}
void *alloc(unsigned int size, unsigned char page_align, HeapStruct *heap)
{

    // Make sure we take the size of header/footer into account.
    unsigned int new_size = size + sizeof(HeaderStruct) + sizeof(FooterStruct);
    // Find the smallest hole that will fit.
    int iterator = FindSmallestHole(new_size, page_align, heap);;

    if(iterator == -1) { // If we didn't find a suitable hole
         // Save some previous data.
         unsigned int old_length = heap->end_address - heap->start_address;
         unsigned int old_end_address = heap->end_address;

         // We need to allocate some more space.
         expand(old_length+new_size, heap);
         unsigned int new_length = heap->end_address-heap->start_address;

         // Find the endmost header. (Not endmost in size, but in location).
         iterator = 0;
         // Vars to hold the index of, and value of, the endmost header found so far.
         unsigned int idx = -1; unsigned int value = 0x0;
         while (iterator < heap->index.size) {
              unsigned int tmp = (unsigned int)OrderedArrayLookup(iterator, &heap->index);
              if (tmp > value) {
                   value = tmp;
                   idx = iterator;
              }
              iterator++;
         }
         // If we didn't find ANY headers, we need to add one.
         if (idx == -1) {
              HeaderStruct *header = (HeaderStruct *)old_end_address;
              header->magic = HEAP_MAGIC;
              header->size = new_length - old_length;
              header->is_hole = 1;
              FooterStruct *footer = (FooterStruct *) (old_end_address + header->size - sizeof(FooterStruct));
              footer->magic = HEAP_MAGIC;
              footer->header = header;
              OrderedArrayInsert((void*)header, &heap->index);
         } else {
              // The last header needs adjusting.
              HeaderStruct *header = OrderedArrayLookup(idx, &heap->index);
              header->size += new_length - old_length;
              // Rewrite the footer.
              FooterStruct *footer = (FooterStruct *) ((unsigned int)header + header->size - sizeof(FooterStruct));
              footer->header = header;
              footer->magic = HEAP_MAGIC;
         }
         // We now have enough space. Recurse, and call the function again.
         return alloc(size, page_align, heap);
    }

    HeaderStruct *orig_hole_header = (HeaderStruct *)OrderedArrayLookup(iterator, &heap->index);
    unsigned int orig_hole_pos = (unsigned int)orig_hole_header;
    unsigned int orig_hole_size = orig_hole_header->size;
    // Here we work out if we should split the hole we found into two parts.
    // Is the original hole size - requested hole size less than the overhead for adding a new hole?
    if (orig_hole_size-new_size < sizeof(HeaderStruct)+sizeof(FooterStruct)) {
         // Then just increase the requested size to the size of the hole we found.
         size += orig_hole_size-new_size;
         new_size = orig_hole_size;
    }

    // If we need to page-align the data, do it now and make a new hole in front of our block.
    if (page_align && (orig_hole_pos&0xFFFFF000)) {
         unsigned int new_location   = orig_hole_pos + PAGE_SIZE - (orig_hole_pos&0xFFF) - sizeof(HeaderStruct);
         HeaderStruct *hole_header = (HeaderStruct *)orig_hole_pos;
         hole_header->size     = PAGE_SIZE - (orig_hole_pos&0xFFF) - sizeof(HeaderStruct);
         hole_header->magic    = HEAP_MAGIC;
         hole_header->is_hole  = 1;
         FooterStruct *hole_footer = (FooterStruct *) ((unsigned int)new_location - sizeof(FooterStruct));
         hole_footer->magic    = HEAP_MAGIC;
         hole_footer->header   = hole_header;
         orig_hole_pos         = new_location;
         orig_hole_size        = orig_hole_size - hole_header->size;
    } else {
         // Else we don't need this hole any more, delete it from the index.
         OrderedArrayRemove(iterator, &heap->index);
    }

    // Overwrite the original header...
    HeaderStruct *block_header  = (HeaderStruct *)orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    // ...And the footer
    FooterStruct *block_footer  = (FooterStruct *) (orig_hole_pos + sizeof(HeaderStruct) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;

    // We may need to write a new hole after the allocated block.
    // We do this only if the new hole would have positive size...
    if (orig_hole_size - new_size > 0) {
         HeaderStruct *hole_header = (HeaderStruct *) (orig_hole_pos + sizeof(HeaderStruct) + size + sizeof(FooterStruct));
         hole_header->magic    = HEAP_MAGIC;
         hole_header->is_hole  = 1;
         hole_header->size     = orig_hole_size - new_size;
         FooterStruct *hole_footer = (FooterStruct *) ((unsigned int)hole_header + orig_hole_size - new_size - sizeof(FooterStruct));
         if (((unsigned int)hole_footer+sizeof(FooterStruct)) <= heap->end_address) { // BUG
              hole_footer->magic = HEAP_MAGIC;
              hole_footer->header = hole_header;
         }
         // Put the new hole in the index;
         OrderedArrayInsert((void*)hole_header, &heap->index);
    }
    // ...And we're done!
    return (void *) ( (unsigned int)block_header+sizeof(HeaderStruct));
}

void free(void *p, HeapStruct *heap)
{
    // Exit gracefully for null pointers.
    if (p == 0)
         return;

    // Get the header and footer associated with this pointer.
    HeaderStruct *header = (HeaderStruct*) ((unsigned int)p - sizeof(HeaderStruct));
    FooterStruct *footer = (FooterStruct*) ((unsigned int)header + header->size - sizeof(FooterStruct));

    // Sanity checks.
    KASSERT(header->magic == HEAP_MAGIC);
    KASSERT(footer->magic == HEAP_MAGIC);

    // Make us a hole.
    header->is_hole = 1;

    // Do we want to add this header into the 'free holes' index?
    char do_add = 1;

    // Unify left
    // If the thing immediately to the left of us is a footer...
    FooterStruct *test_footer = (FooterStruct*)((unsigned int)header - sizeof(FooterStruct));
    if (test_footer->magic == HEAP_MAGIC &&
        test_footer->header->is_hole == 1) {
         unsigned int cache_size = header->size; // Cache our current size.
         header = test_footer->header;     // Rewrite our header with the new one.
         footer->header = header;          // Rewrite our footer to point to the new header.
         header->size += cache_size;       // Change the size.
         do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
    }

    // Unify right
    // If the thing immediately to the right of us is a header...
    HeaderStruct *test_header = (HeaderStruct*)((unsigned int)footer + sizeof(FooterStruct));
    if (test_header->magic == HEAP_MAGIC &&
        test_header->is_hole) {
         header->size += test_header->size; // Increase our size.
         test_footer = (FooterStruct*) ((unsigned int)test_header + // Rewrite it's footer to point to our header.
                                    test_header->size - sizeof(FooterStruct) );
         footer = test_footer;
         // Find and remove this header from the index.
         unsigned int iterator = 0;
         while ((iterator < heap->index.size) &&
                (OrderedArrayLookup(iterator, &heap->index) != (void*)test_header))
              iterator++;

         // Make sure we actually found the item.
         KASSERT(iterator < heap->index.size);
         // Remove it.
         OrderedArrayRemove(iterator, &heap->index);
    }

    // If the footer location is the end address, we can contract.
    if ((unsigned int)footer+sizeof(FooterStruct) == heap->end_address) {
         unsigned int old_length = heap->end_address-heap->start_address;
         unsigned int new_length = contract((unsigned int)header - heap->start_address, heap);
         // Check how big we will be after resizing.
         if (header->size - (old_length-new_length) > 0) {
              // We will still exist, so resize us.
              header->size -= old_length-new_length;
              footer = (FooterStruct*) ((unsigned int)header + header->size - sizeof(FooterStruct));
              footer->magic = HEAP_MAGIC;
              footer->header = header;
         } else {
              // We will no longer exist :(. Remove us from the index.
              unsigned int iterator = 0;
              while ((iterator < heap->index.size) &&
                    (OrderedArrayLookup(iterator, &heap->index) != (void*)test_header))
                   iterator++;
              // If we didn't find ourselves, we have nothing to remove.
              if (iterator < heap->index.size)
                   OrderedArrayRemove(iterator, &heap->index);
         }
    }
    // If required, add us to the index.
    if (do_add == 1)
        OrderedArrayInsert((void*)header, &heap->index);
}
