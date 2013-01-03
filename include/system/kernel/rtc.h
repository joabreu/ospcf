/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\rtc.h
    @brief Real time clock
*/
#ifndef INCLUDE_SYSTEM_KERNEL_RTC_H
#define INCLUDE_SYSTEM_KERNEL_RTC_H

/** @brief RTC Command Port */
#define RTC_COMMAND_PORT		0x70
/** @brief RTC Data Port */
#define RTC_DATA_PORT			0x71

/** @brief Command: Get Second */
#define RTC_SECOND		0x00
/** @brief Command: Get Minute */
#define RTC_MINUTE		0x02
/** @brief Command: Get Hour */
#define RTC_HOUR		0x04
/** @brief Command: Get Day of week */
#define RTC_DAYOFWEEK	0x06
/** @brief Command: Get Day of month */
#define RTC_DAYOFMONTH	0x07
/** @brief Command: Get Month number */
#define RTC_MONTH		0x08
/** @brief Command: Get Year */
#define RTC_YEAR		0x09
/** @brief Command: Get Century */
#define RTC_CENTURY		0x32
/** @brief Command: Get StatusA register value */
#define RTC_STATUSA		0x0A
/** @brief Command: Get StatusB register value */
#define RTC_STATUSB		0x0B
/** @brief Command: Get StatusC register value */
#define RTC_STATUSC		0x0C

/** @brief RTC current time structure */
typedef struct _RtcTimeStruct
{
	/** @brief Day of week */
	unsigned long int DayWeek:3;
	/** @brief Day of month */
	unsigned long int DayMonth:5;
	/** @brief Month number */
	unsigned char Month;
	/** @brief Year number */
	unsigned short int Year;

	/** @brief Second */
	unsigned char Second;
	/** @brief Minute */
	unsigned char Minute;
	/** @brief Hour */
	unsigned char Hour;
} RtcTimeStruct __attribute__ ((packed));

/** @fn void SysSetBootTime(RtcTimeStruct time)
    @brief Set kernel boot time. Defined in SystemStatus.KernelBootTime
    @ingroup sys_kernel
    @param time Time
*/
void SysSetBootTime(RtcTimeStruct time);
/** @fn RtcTimeStruct SysGetBootTime()
    @brief Return kernel boot time
    @ingroup sys_kernel
    @return Kernel boot time
*/
RtcTimeStruct SysGetBootTime();
/** @fn int SysGetDate(RtcTimeStruct *time)
    @brief Get RTC time/date
    @ingroup sys_kernel
    @param time Data destination
    @return Error code
*/
int SysGetDate(RtcTimeStruct *time);
/** @fn int SysSetDate(RtcTimeStruct time)
    @brief Set RTC time/date
    @ingroup sys_kernel
    @param time New RTC time
    @return Error code
    @todo Implement this
*/
int SysSetDate(RtcTimeStruct time);
/** @fn signed long int SysGetRtcUpTime()
    @brief Return seconds passed since kernel boot
    @ingroup sys_kernel
    @return Seconds
*/
signed long int SysGetRtcUpTime();
/** @fn signed long int SysGetRtcSeconds()
    @brief Return seconds passed since 1970, using function maketime()
    @ingroup sys_kernel
    @return Seconds
*/
signed long int SysGetRtcSeconds();
/** @fn int RtcInit(signed char Cmd)
    @brief Init Real Time Clock
    @ingroup sys_kernel
    @param Cmd If false install IRQ handler as well
    @return Error code
*/
int RtcInit(signed char Cmd);

#endif
