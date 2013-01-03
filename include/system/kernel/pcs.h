/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\pcs.h
    @brief PC speaker
*/
#ifndef INCLUDE_SYSTEM_KERNEL_PCS_H
#define INCLUDE_SYSTEM_KERNEL_PCS_H

/** @fn void PcsSound(unsigned long int nFrequence)
    @brief Turn PC speaker on
    @ingroup sys_kernel
    @param nFrequence Frequence
*/
void PcsSound(unsigned long int nFrequence);
/** @fn void PcsNoSound()
    @brief Turn PC speaker off
    @ingroup sys_kernel
*/
void PcsNoSound();
/** @fn void PcsNoise()
    @brief Make a small noise by turning the speaker on and off fast
    @ingroup sys_kernel
*/
void PcsNoise();

#endif
