/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\rtc.c
    @brief Real time clock
*/
#include <errno.h>
#include <string.h>
#include <time.h>
#include <kernel\rtc.h>
#include <kernel\status.h>
#include <kernel\x86\io.h>
#include <kernel\x86\irq.h>

/** @brief Convert x to bit
    @param x Char
    @return Bit
*/
#define BIT(x)				(1 << (x))
/** @brief Convert an BCD8 to decimal 
    @param a BCD8
    @return Decimal
*/
#define BCD8_TO_DECIMAL(a) 	((( ((a) & 0xF0 ) >> 4) * 10) + ((a) & 0xF))

/** @brief If TRUE the component is inited */
static char RtcInited = 0;

void SysSetBootTime(RtcTimeStruct time)
{
    SystemStatus.KernelBootTime = time;
}
RtcTimeStruct SysGetBootTime()
{
    return SystemStatus.KernelBootTime;
}
int SysGetDate(RtcTimeStruct *time)
{
	if(!time)
		 return -EINVAL;
	memset(time, 0, sizeof(RtcTimeStruct));		
    
    if(!RtcInited)
		 return -EINITFRST;

	// Lock Spin Lock
	outportb(RTC_COMMAND_PORT, RTC_DAYOFWEEK);
	time->DayWeek = inportb(RTC_DATA_PORT);
	outportb(RTC_COMMAND_PORT, RTC_DAYOFMONTH );
	time->DayMonth = BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT));
	outportb(RTC_COMMAND_PORT, RTC_MONTH);
	time->Month = BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT));
	outportb(RTC_COMMAND_PORT, RTC_YEAR);
	time->Year = BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT));
	outportb(RTC_COMMAND_PORT, RTC_CENTURY);
	time->Year += BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT)) * 100;

	outportb(RTC_COMMAND_PORT, RTC_SECOND );
	time->Second = BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT));
	outportb(RTC_COMMAND_PORT, RTC_MINUTE);
	time->Minute = BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT));
	outportb(RTC_COMMAND_PORT, RTC_HOUR);
	time->Hour = BCD8_TO_DECIMAL(inportb(RTC_DATA_PORT));

	// Unlock
	return ESUCCESS;
}
int SysSetDate(RtcTimeStruct time)
{
	// Not implemented
    return -ENOTSUP;
}
signed long int SysGetRtcUpTime()
{
	RtcTimeStruct time;
	RtcTimeStruct boot = SysGetBootTime();
	signed long int temp;
	signed long int origin;
	signed long int difference;

	if(SysGetDate(&time) != ESUCCESS)
		 return -EIO;
 
	temp = (60 * 60 * time.Hour) + (60 * time.Minute) + (time.Second);
    origin = (60 * 60 * boot.Hour) + (60 * boot.Minute) + (boot.Second);
    difference = temp - origin;
    
	return difference;
}
signed long int SysGetRtcSeconds()
{
	RtcTimeStruct time;

	if(SysGetDate(&time) !=  ESUCCESS)
		 return -EIO;
	return maketime(time.Year, time.Month, time.DayMonth, time.Hour, time.Minute, time.Second);
}
/** @fn void RtcIrqHandler(x86RegistersStruct *r)
    @brief RTC internal IRQ handler
    @ingroup sys_kernel
    @param r CPU registers
    @internal
*/
void RtcIrqHandler(x86RegistersStruct *r)
{
	unsigned char i;

	outportb(RTC_COMMAND_PORT, RTC_STATUSC);
	i = inportb(RTC_DATA_PORT);
}
int RtcInit(signed char Cmd)
{
	unsigned char status_a, status_b, status_c;

	if(Cmd)
    {
		RtcInited = 1;
		return ESUCCESS;
	}

	x86IrqInstallHandler(8, RtcIrqHandler);
	// Read Status A
	outportb(RTC_COMMAND_PORT, RTC_STATUSA);
	status_a = inportb(RTC_DATA_PORT);
	// Read Status B
	outportb(RTC_COMMAND_PORT, RTC_STATUSB);
	status_b = inportb(RTC_DATA_PORT);
	// Read Status C
	outportb(RTC_COMMAND_PORT, RTC_STATUSC);
	status_c = inportb(RTC_DATA_PORT);

	// Enable periodic interrupt
	status_b |= BIT(6);

	// Write Status B
	outportb(RTC_COMMAND_PORT, RTC_STATUSB);
	outportb(RTC_DATA_PORT, status_b);
	// Write Status C
	outportb(RTC_COMMAND_PORT, RTC_STATUSC);
	outportb(RTC_DATA_PORT, status_c);

	RtcInited = 1; 
	return ESUCCESS;
}
