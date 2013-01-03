/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\pcs.c
    @brief PC speaker
*/
#include <kernel\timer.h>
#include <kernel\x86\io.h>

void PcsSound(unsigned long int nFrequence)
{
    unsigned long int nFreqDiv;
	unsigned char nTmp;

	nFreqDiv = 1193180 / nFrequence;
	outportb(0x43, 0xb6);
	outportb(0x42,(unsigned char)(nFreqDiv));
	outportb(0x42,(unsigned char)(nFreqDiv >> 8));
	nTmp = inportb(0x61);
	if (nTmp!=(nTmp | 3))
		 outportb(0x61, nTmp | 3);
}
void PcsNoSound()
{
	unsigned char nTmp = inportb(0x61) & 0xFC;
	outportb(0x61, nTmp);
}
void PcsNoise()
{
	PcsSound(1000);
	TimerDelay(10);
	PcsNoSound();
}
