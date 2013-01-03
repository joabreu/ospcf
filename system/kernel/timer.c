/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\timer.c
    @brief Kernel timer
*/
#include <kernel\console.h>
#include <kernel\status.h>
#include <kernel\proc.h>
#include <kernel\x86\io.h>
#include <kernel\x86\irq.h>
#include <drivers\video\vga.h>

/** @brief Current Timer Frequency in Hz */
static unsigned int TimerFrequency = 0;
/** @brief Seconds passed since timer was inited */
static volatile unsigned long SystemSeconds = 0;
/** @brief How many times the timer handler was executed */
static volatile unsigned long SystemTicks = 0;
/** @brief Pointer position, just for fun */
static volatile unsigned int Pointer = 0;
/** @brief Pointer colour */
static int att = COLOUR_WHITE + (COLOUR_LIGHTGREEN << 4) + (1 << 7);

unsigned long GetSystemSeconds()
{
    return SystemSeconds;
}
unsigned long GetSystemTicks()
{
    return SystemTicks;
}
unsigned int TimerGetFrequency()
{
    return TimerFrequency;
}
/** @fn void TimerHandler(x86RegistersStruct *r)
    @brief Timer IRQ C handler
    @ingroup sys_kernel
    @param r CPU registers status
    @internal
*/
void TimerHandler(x86RegistersStruct *r)
{	////////////////////// HMM... BEST PLACE ?
    ProcessStruct *curr;
	curr = GetCurrentProcess();
	if(r->cs == 0x1b)
    {
		 curr->utime++;
		 curr->tcurrent->utime++;
	}
    else
    {
		 curr->stime++;
		 curr->tcurrent->stime++;
	}
	//////////////////////////////
	
    SystemTicks++;
    if(SystemTicks % TimerFrequency == 0)
    {
         SystemSeconds++;
        
         if(VgaCurrentMode.text_mode == true)
         {
              Pointer++;
              if(Pointer > 3) 
                   Pointer = 0;
              switch(Pointer)
              {
                   case 0:*(VgaCurrentMode.memory + (( 0 * VgaCurrentMode.width ) + (VgaCurrentMode.width - 1))) = '|' | (att << 8 );break;
                   case 1:*(VgaCurrentMode.memory + (( 0 * VgaCurrentMode.width ) + (VgaCurrentMode.width - 1))) = '/' | (att << 8 );break;
                   case 2:*(VgaCurrentMode.memory + (( 0 * VgaCurrentMode.width ) + (VgaCurrentMode.width - 1))) = '-' | (att << 8 );break;
                   case 3:*(VgaCurrentMode.memory + (( 0 * VgaCurrentMode.width ) + (VgaCurrentMode.width - 1))) = '\\' | (att << 8 );break;
              }
         }
    }
}
void TimerDelays(unsigned long seconds)
{
    unsigned long new_count;
    new_count = SystemSeconds + seconds;
    while(SystemSeconds < new_count)
         ;
}
void TimerDelay(unsigned long ticks)
{
    unsigned long new_count;
    new_count = SystemTicks + ticks;
    while(SystemTicks < new_count)
         ;
}
void TimerSetFrequency(unsigned int freq)
{
    TimerFrequency = freq;
    long temp = 0x1234DD / freq;
    outportb(0x43, 0x36);          //Channel 0 - Mode 3 - Binary
    outportb(0x40, temp & 0xFF);   //Lo Byte            //99.97 Hz
    outportb(0x40, temp >> 8);     //Hi Byte
}
int TimerInstall(unsigned int freq)
{
    TimerSetFrequency(freq);
    return x86IrqInstallHandler(0, TimerHandler);
}
