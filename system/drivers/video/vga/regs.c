/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\regs.c
    @brief VGA driver
*/
#include <kernel\x86\io.h>
#include <drivers\video\vga.h>

void VgaReadRegs(unsigned char *regs)
{
	unsigned i;

    // read MISCELLANEOUS reg
	*regs = inportb(VGA_MISC_READ);
	regs++;
    
    // read SEQUENCER regs
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		 outportb(VGA_SEQ_INDEX, i);
		 *regs = inportb(VGA_SEQ_DATA);
		 regs++;
	}
    
    // read CRTC regs
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		 outportb(VGA_CRTC_INDEX, i);
		 *regs = inportb(VGA_CRTC_DATA);
		 regs++;
	}
    
    // read GRAPHICS CONTROLLER regs
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		 outportb(VGA_GC_INDEX, i);
		 *regs = inportb(VGA_GC_DATA);
		 regs++;
	}
    
    // read ATTRIBUTE CONTROLLER regs
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		 (void)inportb(VGA_INSTAT_READ);
		 outportb(VGA_AC_INDEX, i);
		 *regs = inportb(VGA_AC_READ);
		 regs++;
	}
    
    // lock 16-color palette and unblank display
	(void)inportb(VGA_INSTAT_READ);
	outportb(VGA_AC_INDEX, 0x20);
}
void VgaWriteRegs(unsigned char *regs)
{
	unsigned i;

    // write MISCELLANEOUS reg
	outportb(VGA_MISC_WRITE, *regs);
	regs++;
    
    // write SEQUENCER regs
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		 outportb(VGA_SEQ_INDEX, i);
		 outportb(VGA_SEQ_DATA, *regs);
		 regs++;
	}
    
    // unlock CRTC registers
	outportb(VGA_CRTC_INDEX, 0x03);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) | 0x80);
	outportb(VGA_CRTC_INDEX, 0x11);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) & ~0x80);
    
    // make sure they remain unlocked
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
    
    // write CRTC regs
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		 outportb(VGA_CRTC_INDEX, i);
		 outportb(VGA_CRTC_DATA, *regs);
		 regs++;
	}
	
    // write GRAPHICS CONTROLLER regs
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		 outportb(VGA_GC_INDEX, i);
		 outportb(VGA_GC_DATA, *regs);
		 regs++;
	}

    // write ATTRIBUTE CONTROLLER regs
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		 (void)inportb(VGA_INSTAT_READ);
		 outportb(VGA_AC_INDEX, i);
		 outportb(VGA_AC_WRITE, *regs);
		 regs++;
	}
    
    // lock 16-color palette and unblank display
	(void)inportb(VGA_INSTAT_READ);
	outportb(VGA_AC_INDEX, 0x20);
}
