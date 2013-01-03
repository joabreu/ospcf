/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\timer.h
    @brief Kernel Timer
*/
#ifndef INCLUDE_SYSTEM_KERNEL_TIMER_H
#define INCLUDE_SYSTEM_KERNEL_TIMER_H

/** @fn unsigned long GetSystemSeconds()
    @brief Return seconds passed since timer was inited
    @ingroup sys_kernel
    @return Seconds
*/
unsigned long GetSystemSeconds();
/** @fn unsigned long GetSystemTicks()
    @brief Return ticks passed since timer was inited. The number of times that TimerHandler() was called
    @ingroup sys_kernel
    @return Ticks
*/
unsigned long GetSystemTicks();
/** @fn unsigned int TimerGetFrequency()
    @brief Return current timer frequency in Hz
    @ingroup sys_kernel
    @return Frequency
*/
unsigned int TimerGetFrequency();
/** @fn void TimerDelays(unsigned long seconds)
    @brief Loops during seconds seconds
    @ingroup sys_kernel
    @param seconds Seconds
*/
void TimerDelays(unsigned long seconds);
/** @fn void TimerDelay(unsigned long ticks)
    @brief Loops during ticks ticks
    @ingroup sys_kernel
    @param ticks Ticks
*/
void TimerDelay(unsigned long ticks);
/** @fn void TimerSetFrequency(unsigned int freq)
    @brief Set timer frequency
    @ingroup sys_kernel
    @param freq New frequency in Hz
*/
void TimerSetFrequency(unsigned int freq);
/** @fn int TimerInstall(unsigned int freq)
    @brief Install system timer. Tries to regist it to IRQ 0
    @ingroup sys_kernel
    @param freq Timer frequency
    @return Error code
*/
int TimerInstall(unsigned int freq);

#endif
