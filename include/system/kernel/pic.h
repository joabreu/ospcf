/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\pic.h
    @brief PIC
*/
#ifndef INCLUDE_SYSTEM_KERNEL_PIC_H
#define INCLUDE_SYSTEM_KERNEL_PIC_H

/** @brief Master PIC port */
#define MASTER_PIC 0x20
/** @brief Slave PIC port */
#define SLAVE_PIC  0xa0

/** @fn void PicRemap(unsigned char start)
    @brief Remap IRQ 0 to 15
    @ingroup sys_kernel
    @param start New start
*/
void PicRemap(unsigned char start);
/** @fn void PicEnableIrq(int num)
    @brief Enables interrupts for this IRQ
    @ingroup sys_kernel
    @param num IRQ number
*/
void PicEnableIrq(int num);
/** @fn void PicDisableIrq(int num)
    @brief Disables interrupts for this IRQ
    @ingroup sys_kernel
    @param num IRQ number
*/
void PicDisableIrq(int num);

#endif
