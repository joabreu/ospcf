/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\idt.h
    @brief Interrupt Descriptor Table includes
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_IDT_H
#define INCLUDE_SYSTEM_KERNEL_X86_IDT_H

/** @brief Number of IDT entries in the system */
#define SYSTEM_IDT_NENTRIES 256

/** @brief Single IDT (Interrupt Descriptor Table) entry struct */
struct x86IdtEntryStruct
{
    /** @brief The lower 16 bits of the base */
    unsigned short BaseLow;
    /** @brief Selector */
    unsigned short Sel;
    /** @brief Always Zero Field */
    unsigned char Always0;
    /** @brief Entry Flags */
    unsigned char Flags;
    /** @brief The last 8 bits of the base */
    unsigned short BaseHigh;
} __attribute__((packed));

/** @brief Pointer to a valid IDT (Interrupt Descriptor Table) */
struct x86IdtPtrStruct
{
    /** @brief IDT end */
    unsigned short Limit;
    /** @brief Address of first segment */
    unsigned long Base;
} __attribute__((packed));

/** @fn void x86IdtSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
    @brief Sets a new gate in the IDT
    @ingroup sys_kernel_arch_x86
    @param num Gate number
    @param base Base value
    @param sel Segment
    @param flags Gate flags
*/
void x86IdtSetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
/** @fn void x86IdtInstall()
    @brief Install system IDT
    @ingroup sys_kernel_arch_x86
*/
void x86IdtInstall();

#endif
