/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\gdt.h
    @brief Global Descriptor Table includes
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_GDT_H
#define INCLUDE_SYSTEM_KERNEL_X86_GDT_H

/** @brief Number of GDT entries in the system */
#define SYSTEM_GDT_NENTRIES 6

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
/** @brief Single GDT (Global Descriptor Table) entry struct */
struct x86GdtEntryStruct
{
    /** @brief The lower 16 bits of the limit */
    unsigned short limit_low;
    /** @brief The lower 16 bits of the base */
    unsigned short base_low;
    /** @brief The next 8 bits of the base */
    unsigned char  base_middle;
    /** @brief Access flags, determine what ring this segment can be used in */
    unsigned char  access;
    /** @brief Segment granularity */
    unsigned char  granularity;
    /** @brief The last 8 bits of the base */
    unsigned char  base_high;
} __attribute__((packed));

/** @brief Pointer to a valid GDT (Global Descriptor Table) */
struct x86GdtPtrStruct
{
    /** @brief GDT end */
    unsigned short limit;
    /** @brief Address of first segment */
    unsigned long base;
} __attribute__((packed));

/** @fn void x86GdtInstall()
    @brief Install system GDT
    @ingroup sys_kernel_arch_x86 
*/
void x86GdtInstall();
/** @fn void x86GdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
    @brief Define a entry in our GDT
    @ingroup sys_kernel_arch_x86 
    @param num Entry number (0 to 5)
    @param base Base value
    @param limit Limit value
    @param access Gate privileges (ring 0, ring 3, ...)
    @param gran Granulaty
*/
void x86GdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

#endif
