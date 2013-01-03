/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\paging.c
    @brief PCF paging system
*/
/************************ PORTED TO PCF ************************/
// paging.c -- Defines the interface for and structures relating to paging.
//             Written for JamesM's kernel development tutorials.
#include <string.h>
#include <kernel\status.h>
#include <kernel\debug.h>
#include <kernel\mem\heap.h>
#include <kernel\mem\kheap.h>
#include <kernel\mem\uheap.h>
#include <kernel\x86\mem.h>
#include <kernel\x86\paging.h>

/** @fn extern void x86CopyPagePhysical(unsigned int src, unsigned int dst)
    @brief Copys the contents of a page to another
    @ingroup sys_kernel_arch_x86 
    @param src Source page address
    @param dst Destination page address
    @internal
*/
extern void x86CopyPagePhysical(unsigned int src, unsigned int dst);

/** @brief Kernel Page Directory */
x86PageDirectoryStruct *KernelDirectory = 0;
/** @brief Current (Running) Page Directory */
x86PageDirectoryStruct *CurrentDirectory = 0;
/** @brief Present Frames */
static unsigned int *Frames;
/** @brief Number of used frames */
static unsigned int NUsedFrames;
/** @brief Total number of existing frames */
static unsigned int NFrames;

extern unsigned long PlacementAddress;     // System end
extern HeapStruct *KernelHeap;             // Kernel Heap used in kmalloc
extern HeapStruct *UserHeap;               // User Heap used in umalloc

/** @fn static void x86PagingSetFrame(unsigned int FrameAddr)
    @brief Marcs a frame as used in the frames bitset
    @ingroup sys_kernel_arch_x86 
    @param FrameAddr Frame Adress
    @internal
*/
static void x86PagingSetFrame(unsigned int FrameAddr)
{
    unsigned int frame = FrameAddr/PAGE_SIZE;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    Frames[idx] |= (0x1 << off);
}
/** @fn static void x86PagingClearFrame(unsigned int FrameAddr)
    @brief Marcs a frame as not used in the frames bitset
    @ingroup sys_kernel_arch_x86 
    @param FrameAddr Frame Adress
    @internal
*/
static void x86PagingClearFrame(unsigned int FrameAddr)
{
    unsigned int frame = FrameAddr/PAGE_SIZE;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    Frames[idx] &= ~(0x1 << off);
}
/** @fn static unsigned int x86PagingTestFrame(unsigned int FrameAddr)
    @brief Tests if a bit is set
    @ingroup sys_kernel_arch_x86 
    @param FrameAddr Frame Adress
    @return TRUE or FALSE
    @internal
*/
static unsigned int x86PagingTestFrame(unsigned int FrameAddr)
{
    unsigned int frame = FrameAddr/PAGE_SIZE;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    return (Frames[idx] & (0x1 << off));
}
/** @fn static unsigned int x86PagingFirstFrame()
    @brief Returns the address of the first free frame in the Frames list
    @ingroup sys_kernel_arch_x86 
    @return First free frame in the list
    @internal
*/
static unsigned int x86PagingFirstFrame()
{
    unsigned int i, j;
    for(i = 0; i < INDEX_FROM_BIT(NFrames); i++) // Loop while free frames
         if(Frames[i] != 0xFFFFFFFF) // nothing free, exit early.
              for(j = 0; j < 32; j++) { // at least one bit is free here.
                   unsigned int toTest = 0x1 << j;
                   if(!(Frames[i]&toTest))
                        return i*4*8+j;
              }
    return 0;
}
void x86PagingAllocFrame(x86PageStruct *Page, int Kernel, int WriteAble)
{
    if(Page->frame != 0)
         return;
    else {
         // Returns free frame address
         unsigned int idx = x86PagingFirstFrame();
         if(idx == (unsigned int)-1)
              PANIC("No Free Frames");
         x86PagingSetFrame(idx*PAGE_SIZE);
         Page->present = 1;
         Page->rw = (WriteAble==1)?1:0;
         Page->user = (Kernel==1)?0:1;
         Page->frame = idx;
         NUsedFrames++;
         SystemStatus.FreePages = NFrames - NUsedFrames;
    }
}
void x86PagingFreeFrame(x86PageStruct *Page)
{
    unsigned int frame;
    if(!(frame=Page->frame)) // If frame is empty return
         return;
    else {
         x86PagingClearFrame(frame);
         Page->frame = 0x0;
         NUsedFrames--;
         SystemStatus.FreePages = NFrames - NUsedFrames;
    }
}
void x86PagingInit(unsigned int MemoryToMap)
{
    // We will only map the memory indicated by MemoryToMap
    SystemStatus.MemoryPaging = MemoryToMap;
    unsigned int MemToMap = MemoryToMap;
    if(MemoryToMap < (4*1024*1024/* 4MB */))
         PANIC("Not enough memory installed, you have %d bytes installed and you need 4MB", MemoryToMap);
    if(MemoryToMap > SystemStatus.MemorySize)
         PANIC("Trying to map %d bytes, but only %d are available", MemoryToMap, SystemStatus.MemorySize);
    
    // Inits free frames
    // Total memory / nframes (4KB)
    NFrames = MemToMap / PAGE_SIZE;
    NUsedFrames = 0;
    SystemStatus.TotalPages = NFrames;
    SystemStatus.FreePages = NFrames;
    Frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(NFrames));
    memset(Frames, 0, INDEX_FROM_BIT(NFrames));
        
    // Let's make a page directory.
    KernelDirectory = (x86PageDirectoryStruct*)kmalloc_a(sizeof(x86PageDirectoryStruct));
    memset(KernelDirectory, 0, sizeof(x86PageDirectoryStruct));
    KernelDirectory->PhysicalAddr = (unsigned int)KernelDirectory->PageTablesPhysical;

    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's 
    // to be created where necessary. We can't allocate frames yet because they
    // need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!
    int i = 0;
    for(i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += PAGE_SIZE)
         x86PagingGetPage(i, 1, KernelDirectory);
    for(i = UHEAP_START; i < UHEAP_START+UHEAP_INITIAL_SIZE; i += PAGE_SIZE)
         x86PagingGetPage(i, 1, KernelDirectory);

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    // Allocate a lil' bit extra so the kernel heap can be
    // initialised properly.
    i = 0;
    while(i < PlacementAddress+PAGE_SIZE) {
         // Kernel code is readable but no writeable from userspace
         /** @todo Warning about this: From  0x0 to PlacementAddress+PAGE_SIZE user's can't read or write. This is kernel reserved */
         x86PagingAllocFrame(x86PagingGetPage(i, 1, KernelDirectory), 1 /*Kernel*/, 0 /*Not write able*/);
         i += PAGE_SIZE;
    }

    // Now allocate those pages we mapped earlier.
    for(i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += PAGE_SIZE)
         x86PagingAllocFrame(x86PagingGetPage(i, 1, KernelDirectory), 1/*Kernel*/, 0/*Not Write able from user*/);
    for(i = UHEAP_START; i < UHEAP_START+UHEAP_INITIAL_SIZE; i += PAGE_SIZE)
         x86PagingAllocFrame(x86PagingGetPage(i, 1, KernelDirectory), 0/*User Page*/, 1/*Write able*/);

    // Now, enable paging!
    x86SwitchPageDirectory(KernelDirectory);
    // Initialise the kernel heap.
    KernelHeap = CreateHeap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, KHEAP_MAX_SIZE, 1/*Kernel*/, 1/*Read-only*/);
    UserHeap = CreateHeap(UHEAP_START, UHEAP_START+UHEAP_INITIAL_SIZE, UHEAP_MAX_SIZE, 0/*User*/, 0/*Write able*/);
    // Clone kernel page
    CurrentDirectory = x86ClonePageDirectory(KernelDirectory);
    x86SwitchPageDirectory(CurrentDirectory);
}
void x86SwitchPageDirectory(x86PageDirectoryStruct *new)
{
    CurrentDirectory = new;
    asm volatile("mov %0, %%cr3":: "r"(new->PhysicalAddr));
    unsigned int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}
x86PageStruct *x86PagingGetPage(unsigned int Address, int Make, x86PageDirectoryStruct *Dir)
{
    // Turn the address into an index.
    Address /= PAGE_SIZE;
    // Find the page table containing this address.
    unsigned int table_idx = Address / 1024;

    if(Dir->PageTables[table_idx]) // If this table is already assigned
         return &Dir->PageTables[table_idx]->pages[Address%1024];
    else if(Make) {
         unsigned int tmp;
         Dir->PageTables[table_idx] = (x86PageTableStruct*)kmalloc_ap(sizeof(x86PageTableStruct), &tmp);
         memset(Dir->PageTables[table_idx], 0, PAGE_SIZE);
         Dir->PageTablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
         return &Dir->PageTables[table_idx]->pages[Address%1024];
    } else
         return 0;
}
/** @fn static x86PageTableStruct *x86ClonePageTable(x86PageTableStruct *src, unsigned int *PhysAddr)
    @brief Clones the contents of a page table
    @ingroup sys_kernel_arch_x86 
    @param src Page Table to clone
    @param PhysAddr Physical address of the new page table (clone)
    @return New page table (clone)
    @internal
*/
static x86PageTableStruct *x86ClonePageTable(x86PageTableStruct *src, unsigned int *PhysAddr)
{
    // Make a new page table, which is page aligned.
    x86PageTableStruct *table = (x86PageTableStruct*)kmalloc_ap(sizeof(x86PageTableStruct), PhysAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(x86PageTableStruct));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++) {
         // If the source entry has a frame associated with it...
         if (!src->pages[i].frame)
              continue;
         // Get a new frame.
         x86PagingAllocFrame(&table->pages[i], 0, 0);
         // Clone the flags from source to destination.
         if (src->pages[i].present) table->pages[i].present = 1;
         if (src->pages[i].rw)      table->pages[i].rw = 1;
         if (src->pages[i].user)    table->pages[i].user = 1;
         if (src->pages[i].accessed)table->pages[i].accessed = 1;
         if (src->pages[i].dirty)   table->pages[i].dirty = 1;
         // Physically copy the data across. This function is in process.s.
         x86CopyPagePhysical(src->pages[i].frame*PAGE_SIZE, table->pages[i].frame*PAGE_SIZE);
    }
    return table;
}
x86PageDirectoryStruct *x86ClonePageDirectory(x86PageDirectoryStruct *src)
{
    unsigned int phys;
    // Make a new page directory and obtain its physical address.
    x86PageDirectoryStruct *dir = (x86PageDirectoryStruct*)kmalloc_ap(sizeof(x86PageDirectoryStruct), &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(x86PageDirectoryStruct));

    // Get the offset of tablesPhysical from the start of the page_directory_t structure.
    unsigned int offset = (unsigned int)dir->PageTablesPhysical - (unsigned int)dir;

    // Then the physical address of dir->tablesPhysical is:
    dir->PhysicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
    int i;
    for (i = 0; i < 1024; i++) {
         if(!src->PageTables[i])
              continue;
         if(KernelDirectory->PageTables[i] == src->PageTables[i]) {
              // It's in the kernel, so just use the same pointer.
              dir->PageTables[i] = src->PageTables[i];
              dir->PageTablesPhysical[i] = src->PageTablesPhysical[i];
         } else {
              // Copy the table.
              unsigned int phys;
              dir->PageTables[i] = x86ClonePageTable(src->PageTables[i], &phys);
              dir->PageTablesPhysical[i] = phys | 0x07;
         }
    }
    return dir;
}
