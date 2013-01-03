/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\paging.h
    @brief PCF paging
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_PAGING_H
#define INCLUDE_SYSTEM_KERNEL_X86_PAGING_H

#include <kernel\x86\irq.h>

/** @brief Bit position. Used in the bitset algorithms */
#define INDEX_FROM_BIT(a)    (a/(8*4))
/** @brief Bit offset. Used in the bitset algorithms */
#define OFFSET_FROM_BIT(a)   (a%(8*4))

/** @brief 4 KB Page struct. Contains Page info */
typedef struct _x86PageStruct
{
    /** @brief Page present in memory. If this is not set writing or reading to this page will cause a Page Fault */
	unsigned int present:1;
	/** @brief Read-only if clear, readwrite if set */
	unsigned int rw:1;
	/** @brief Supervisor level only if clear */
	unsigned int user:1;
	/** @brief Has the page been accessed since last refresh ? */
	unsigned int accessed:1;
	/** @brief Has the page been written since last refresh ? */
	unsigned int dirty:1;
	/** @brief Amalgamation of unused and reserved bits */
	unsigned int unused:7;
	/** @brief Frame address (shifted right 12 bits) */
	unsigned int frame:20;
} x86PageStruct;

/** @brief Page Table struct. Each page table contains 1024 pages (4 MB)*/
typedef struct x86PageTableStruct
{
    /** @brief 1024 page, 4KB each = 4MB */
	x86PageStruct pages[1024];
} x86PageTableStruct;

/** @brief Page Directory struct. Each PD contains 1024 Page Tables (4 GB) */
typedef struct x86PageDirectoryStruct
{
    /** @brief Pointer to page tables */
	x86PageTableStruct *PageTables[1024];
	/** @brief Physical address to page tables */
	unsigned int PageTablesPhysical[1024];
   	/** @brief Physical address of PageTablesPhysical */
	unsigned int PhysicalAddr;
} x86PageDirectoryStruct;

/** @fn void x86PagingAllocFrame(x86PageStruct *Page, int Kernel, int WriteAble)
    @brief Sets the Page as used
    @ingroup sys_kernel_arch_x86
    @param Page Page
    @param Kernel Is this a kernel page ? ( ReadAble from user if = 0 )
    @param WriteAble Is this write able from user ?
*/
void x86PagingAllocFrame(x86PageStruct *Page, int Kernel, int WriteAble);
/** @fn void x86PagingFreeFrame(x86PageStruct *Page)
    @brief Sets the Page free
    @ingroup sys_kernel_arch_x86
    @param Page Page
*/
void x86PagingFreeFrame(x86PageStruct *Page);
/** @fn void x86PagingInit(unsigned int MemoryToMap)
    @brief Init paging. Map MemoryToMap bytes
    @ingroup sys_kernel_arch_x86
    @param MemoryToMap Bytes to map
*/
void x86PagingInit(unsigned int MemoryToMap);
/** @fn void x86SwitchPageDirectory(x86PageDirectoryStruct *new)
    @brief Switch to Page Directory new
    @ingroup sys_kernel_arch_x86
    @param new PD to switch
*/
void x86SwitchPageDirectory(x86PageDirectoryStruct *new);
/** @fn x86PageStruct *x86PagingGetPage(unsigned int Address, int Make, x86PageDirectoryStruct *Dir)
    @brief Return page in which addreess Adress is.
    @ingroup sys_kernel_arch_x86
    @param Address This address is inside the page returned
    @param Make Make this page if it is not present
    @param Dir Page Directory to search in
    @return Page
*/
x86PageStruct *x86PagingGetPage(unsigned int Address, int Make, x86PageDirectoryStruct *Dir);
/** @fn x86PageDirectoryStruct *x86ClonePageDirectory(x86PageDirectoryStruct *src)
    @brief Clones a page directory
    @ingroup sys_kernel_arch_x86
    @param src Original PD
    @return New PD (clone)
*/
x86PageDirectoryStruct *x86ClonePageDirectory(x86PageDirectoryStruct *src);
/** @fn void x86PageFaultHandler(x86RegistersStruct *regs)
    @brief Page fault handler. Clears faulting address and calls debug shell
    @ingroup sys_kernel_arch_x86
    @param regs CPU registers
*/
void x86PageFaultHandler(x86RegistersStruct *regs);

#endif
