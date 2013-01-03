/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\pixel.c
    @brief VGA driver
*/
#include <drivers\video\vga.h>

/** @fn void vpokeb(unsigned off, char val)
    @brief Poke byte to VGA memory
    @ingroup sys_drivers_video_vga
    @param off Destination offset starting from VGA memory
    @param val Value
    @internal
*/
void vpokeb(unsigned off, char val)
{
    char *pokebyte=(char*)(VgaGetSeg()+off);
    *pokebyte=val;
}
/** @fn char vpeekb(unsigned off)
    @brief Get byte from VGA memory
    @ingroup sys_drivers_video_vga
    @param off Origin offset starting from VGA memory
    @return Value
    @internal
*/
char vpeekb(unsigned off)
{
    char *pokebyte=(char*)(VgaGetSeg()+off);
	return (*pokebyte);
}
/** @brief Sets pixel (x,y) with colour c */
void (*VgaDrawPixel)(unsigned x, unsigned y, unsigned c);

void VgaDrawPixel1(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off, mask;

	c = (c & 1) * 0xFF;
	wd_in_bytes = VgaCurrentMode.width / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	vpokeb(off, (vpeekb(off) & ~mask) | (c & mask));
}
void VgaDrawPixel2(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes, off, mask;

	c = (c & 3) * 0x55;
	wd_in_bytes = VgaCurrentMode.width / 4;
	off = wd_in_bytes * y + x / 4;
	x = (x & 3) * 2;
	mask = 0xC0 >> x;
	vpokeb(off, (vpeekb(off) & ~mask) | (c & mask));
}
void VgaDrawPixel4p(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes, off, mask, p, pmask;

	wd_in_bytes = VgaCurrentMode.width / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	pmask = 1;
	for(p = 0; p < 4; p++)
	{
		 VgaSetPlane(p);
		 if(pmask & c)
			  vpokeb(off, vpeekb(off) | mask);
		 else
			  vpokeb(off, vpeekb(off) & ~mask);
		 pmask <<= 1;
	}
}
void VgaDrawPixel8(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = VgaCurrentMode.width;
	off = wd_in_bytes * y + x;
	vpokeb(off, c);
}
void VgaDrawPixel8x(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = VgaCurrentMode.width / 4;
	off = wd_in_bytes * y + x / 4;
	VgaSetPlane(x & 3);
	vpokeb(off, c);
}
