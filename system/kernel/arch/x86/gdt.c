/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\gdt.c
    @brief Global Descriptor Table
*/
#include <kernel\x86\gdt.h>
#include <kernel\x86\tss.h>

/** @fn extern void x86GdtFlush(unsigned long gdtptr)
    @brief Flushes a new GDT into CPU
    @ingroup sys_kernel_arch_x86
    @param gdtptr Pointer to GDT struct
*/
extern void x86GdtFlush(unsigned long gdtptr);

/** @brief Entrys of our GDT (Global Descriptor Table)

    0- Null;
    1- Kernel Code;
    2- Kernel Data;
    3- User Code;
    4- User Data
    5- TSS
*/
struct x86GdtEntryStruct SystemGdtEntries[SYSTEM_GDT_NENTRIES];
/** @brief Pointer to our GDT (Global Descriptor Table) */
struct x86GdtPtrStruct SystemGdtPtr;

void x86GdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    if((num < 0) || (num >= SYSTEM_GDT_NENTRIES))
         return;
         
    SystemGdtEntries[num].base_low    = (base & 0xFFFF);
    SystemGdtEntries[num].base_middle = (base >> 16) & 0xFF;
    SystemGdtEntries[num].base_high   = (base >> 24) & 0xFF;
    SystemGdtEntries[num].limit_low   = (limit & 0xFFFF);
    SystemGdtEntries[num].granularity = (limit >> 16) & 0x0F;
    SystemGdtEntries[num].granularity |= gran & 0xF0;
    SystemGdtEntries[num].access      = access;
}
void x86GdtInstall()
{    
    // Set GDT end address (6 segments)
    SystemGdtPtr.limit = (sizeof(struct x86GdtEntryStruct) * 6) - 1;
    // GDT base
    SystemGdtPtr.base  = (unsigned long)&SystemGdtEntries;

    x86GdtSetGate(0, 0, 0, 0, 0);                // Null segment
    x86GdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment (Ring 0)
    x86GdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment (Ring 0)
    x86GdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // Code segment (Ring 3)
    x86GdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // Data segment (Ring 3)
    
    x86GdtFlush((unsigned long)&SystemGdtPtr);          // Install GDT
    x86TssInit();
}
