/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\idt.c
    @brief Interrupt Descriptor Table
*/
#include <string.h>
#include <kernel\x86\idt.h>

/** @fn extern void x86IdtFlush(unsigned long idtptr)
    @brief Flushes a new IDT into CPU
    @ingroup sys_kernel_arch_x86
    @param idtptr Pointer to IDT struct
*/
extern void x86IdtFlush(unsigned long idtptr);
/** @brief Entrys of our IDT (Interrupt Descriptor Table) (256 Interrupts) */
struct x86IdtEntryStruct SystemIdtEntries[SYSTEM_IDT_NENTRIES];
/** @brief Pointer to our IDT (Interrupt Descriptor Table) */
struct x86IdtPtrStruct   SystemIdtPtr;

void x86IdtSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    SystemIdtEntries[num].BaseLow = base & 0xFFFF;
    SystemIdtEntries[num].BaseHigh = (base >> 16) & 0xFFFF;
    SystemIdtEntries[num].Sel     = sel;
    SystemIdtEntries[num].Always0 = 0;
    SystemIdtEntries[num].Flags   = flags  | 0x60;
}
void x86IdtInstall()
{
    SystemIdtPtr.Limit = sizeof(struct x86IdtEntryStruct) * SYSTEM_IDT_NENTRIES - 1;
    SystemIdtPtr.Base  = (unsigned long)&SystemIdtEntries;

    memset(&SystemIdtEntries, 0, sizeof(struct x86IdtEntryStruct) * SYSTEM_IDT_NENTRIES);
    x86IdtFlush((unsigned long)&SystemIdtPtr);
}
