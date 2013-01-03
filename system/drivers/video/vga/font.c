/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\font.c
    @brief VGA driver
*/
#include <string.h>
#include <kernel\x86\io.h>
#include <drivers\video\vga.h>

/** @fn void vmemwr(unsigned dst_off, unsigned char *src, unsigned count)
    @brief Copy data to VGA memory
    @ingroup sys_drivers_video_vga
    @param dst_off Destination offset starting from VGA memory
    @param src Data origin
    @param count Data count
    @internal
*/
void vmemwr(unsigned dst_off, unsigned char *src, unsigned count)
{
	memcpy((void*)(VgaGetSeg()+dst_off),(void*) src, count);
}
void VgaSetPlane(unsigned p)
{
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
    // set read plane
	outportb(VGA_GC_INDEX, 4);
	outportb(VGA_GC_DATA, p);
    // set write plane
	outportb(VGA_SEQ_INDEX, 2);
	outportb(VGA_SEQ_DATA, pmask);
}
void VgaWriteFont(unsigned char *buf, unsigned font_height)
{
	unsigned char seq2, seq4, gc4, gc5, gc6;
	unsigned i;

    // save registers
    // set_plane() modifies GC 4 and SEQ 2, so save them as well
	outportb(VGA_SEQ_INDEX, 2);
	seq2 = inportb(VGA_SEQ_DATA);

	outportb(VGA_SEQ_INDEX, 4);
	seq4 = inportb(VGA_SEQ_DATA);

    // turn off even-odd addressing (set flat addressing)
    // assume: chain-4 addressing already off
	outportb(VGA_SEQ_DATA, seq4 | 0x04);

	outportb(VGA_GC_INDEX, 4);
	gc4 = inportb(VGA_GC_DATA);

	outportb(VGA_GC_INDEX, 5);
	gc5 = inportb(VGA_GC_DATA);
    // turn off even-odd addressing
	outportb(VGA_GC_DATA, gc5 & ~0x10);

	outportb(VGA_GC_INDEX, 6);
	gc6 = inportb(VGA_GC_DATA);
    // turn off even-odd addressing
	outportb(VGA_GC_DATA, gc6 & ~0x02);
    // write font to plane P4
	VgaSetPlane(2);
    // write font 0
	for(i = 0; i < 256; i++)
	{
		vmemwr(16384u * 0 + i * 32, buf, font_height);
		buf += font_height;
	}
#if 0
    // write font 1
	for(i = 0; i < 256; i++)
	{
		vmemwr(16384u * 1 + i * 32, buf, font_height);
		buf += font_height;
	}
#endif
    // restore registers
	outportb(VGA_SEQ_INDEX, 2);
	outportb(VGA_SEQ_DATA, seq2);
	outportb(VGA_SEQ_INDEX, 4);
	outportb(VGA_SEQ_DATA, seq4);
	outportb(VGA_GC_INDEX, 4);
	outportb(VGA_GC_DATA, gc4);
	outportb(VGA_GC_INDEX, 5);
	outportb(VGA_GC_DATA, gc5);
	outportb(VGA_GC_INDEX, 6);
	outportb(VGA_GC_DATA, gc6);
}
