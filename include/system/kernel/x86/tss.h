/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\tss.h
    @brief Task Segment Selector
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_TSS_H
#define INCLUDE_SYSTEM_KERNEL_X86_TSS_H

/** @brief Structure of a Task State Segment */
struct x86TssEntryStruct
{
    /** @brief The previous TSS - if we used hardware task switching this would form a linked list. */
    unsigned short prev_tss;
    /** @brief Reserved 1 */
    unsigned short res1;
    /** @brief The stack pointer to load when we change to kernel mode. */
    unsigned long esp0;
    /** @brief The stack segment to load when we change to kernel mode. */
    unsigned short ss0;
    /** @brief Reserved 2 */
    unsigned short res2;
    /** @brief Unused */
    unsigned long esp1;
    /** @brief SS1 */
    unsigned short ss1;
    /** @brief Reserved 3 */
    unsigned short res3;
    /** @brief ESP 2 */
    unsigned long esp2; 
    /** @brief SS 2 */ 
    unsigned short ss2;
    /** @brief Reserved 4 */
    unsigned short res4; 
    /** @brief CR3 */  
    unsigned long cr3;
    /** @brief EIP */   
    unsigned long eip;
    /** @brief EFLAGS */   
    unsigned long eflags;
    /** @brief EAX */
    unsigned long eax;
    /** @brief ECX */
    unsigned long ecx;
    /** @brief EDX */
    unsigned long edx;
    /** @brief EBX */
    unsigned long ebx;
    /** @brief ESP */
    unsigned long esp;
    /** @brief EBP */
    unsigned long ebp;
    /** @brief ESI */
    unsigned long esi;
    /** @brief EDI */
    unsigned long edi;
    /** @brief The value to load into ES when we change to kernel mode. */
    unsigned short es;
    /** @brief Reserved 5 */
    unsigned short res5;
    /** @brief The value to load into CS when we change to kernel mode. */
    unsigned short cs;
    /** @brief Reserved 6 */
    unsigned short res6;
    /** @brief The value to load into SS when we change to kernel mode. */
    unsigned short ss;
    /** @brief Reserved 7 */
    unsigned short res7;
    /** @brief The value to load into DS when we change to kernel mode. */
    unsigned short ds;
    /** @brief Reserved 8 */
    unsigned short res8;
    /** @brief The value to load into FS when we change to kernel mode. */
    unsigned short fs;
    /** @brief Reserved 9 */
    unsigned short res9;
    /** @brief The value to load into GS when we change to kernel mode. */
    unsigned short gs;
    /** @brief Reserved 10 */
    unsigned short res10;
    /** @brief Unused */
    unsigned short ldt;
    /** @brief Reserved 11 */
    unsigned short res11;
    /** @brief Reserved 12 */
    unsigned short res12;
    /** @brief IO Map base */
    unsigned short iomap_base;
} __attribute__((packed));

/** @brief System TSS struct. VERY IMPORTANT. Used to run Ring3 tasks */
extern struct x86TssEntryStruct SystemTssEntry;

/** @fn void x86SetKernelStack(unsigned long stack)
    @brief Sets the value SystemTssEntry.esp0
    @ingroup sys_kernel_arch_x86 
    @param stack New esp0 value
*/
void x86SetKernelStack(unsigned long stack);
/** @fn void x86TssInit()
    @brief Inits system TSS
    @ingroup sys_kernel_arch_x86 
*/
void x86TssInit();

#endif
