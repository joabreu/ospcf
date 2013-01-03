/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\setmode.c
    @brief VGA driver
*/
#include <errno.h>
#include <kernel\console.h>
#include <kernel\x86\io.h>
#include <drivers\video\vga.h>

/** @fn extern void VgaDrawPixel1(unsigned x, unsigned y, unsigned c)
    @brief VGA Draw Pixel 1 
    @ingroup sys_drivers_video_vga 
    @param x X Coordinate
    @param y Y Coordinate
    @param c Colour
    @internal
*/
extern void VgaDrawPixel1(unsigned x, unsigned y, unsigned c);
/** @fn extern void VgaDrawPixel2(unsigned x, unsigned y, unsigned c)
    @brief VGA Draw Pixel 2
    @ingroup sys_drivers_video_vga 
    @param x X Coordinate
    @param y Y Coordinate
    @param c Colour
    @internal
*/
extern void VgaDrawPixel2(unsigned x, unsigned y, unsigned c);
/** @fn extern void VgaDrawPixel4p(unsigned x, unsigned y, unsigned c)
    @brief VGA Draw Pixel 4p
    @ingroup sys_drivers_video_vga 
    @param x X Coordinate
    @param y Y Coordinate
    @param c Colour
    @internal
*/
extern void VgaDrawPixel4p(unsigned x, unsigned y, unsigned c);
/** @fn extern void VgaDrawPixel8(unsigned x, unsigned y, unsigned c)
    @brief VGA Draw Pixel 8
    @ingroup sys_drivers_video_vga 
    @param x X Coordinate
    @param y Y Coordinate
    @param c Colour
    @internal
*/
extern void VgaDrawPixel8(unsigned x, unsigned y, unsigned c);
/** @fn extern void VgaDrawPixel8x(unsigned x, unsigned y, unsigned c)
    @brief VGA Draw Pixel 8x
    @ingroup sys_drivers_video_vga 
    @param x X Coordinate
    @param y Y Coordinate
    @param c Colour
    @internal
*/
extern void VgaDrawPixel8x(unsigned x, unsigned y, unsigned c);
/** @brief Poke Byte
    @param S Segment
    @param O Offset
    @param V Value
*/
#define	pokeb(S,O,V)		*(unsigned char *)(16uL * (S) + (O)) = (V)
/** @brief Poke Word
    @param S Segment
    @param O Offset
    @param V Value
*/
#define	pokew(S,O,V)		*(unsigned short *)(16uL * (S) + (O)) = (V)

int VgaSetMode(int mode)
{
    if(mode == VgaCurrentMode.mode)
         return ESUCCESS; // Mode is already set
         
    switch(mode)
    {
         case VGA_80x25_TEXT: // 80x25 Text Mode
              VgaWriteRegs(Vga80x25_Text);
              VgaCurrentMode.mode      = VGA_80x25_TEXT;
              VgaCurrentMode.width     = 80;
              VgaCurrentMode.height    = 25;
              VgaCurrentMode.memory    = (unsigned short *)0x000B8000;
              VgaCurrentMode.text_mode = true;
              VgaWriteFont(Vga8x16_Font, 16);
              ConsoleInitCursor();
              break;
         case VGA_80x50_TEXT: // 80x50 Text Mode
              VgaWriteRegs(Vga80x50_Text);
              VgaCurrentMode.mode      = VGA_80x50_TEXT;
              VgaCurrentMode.width     = 80;
              VgaCurrentMode.height    = 50;
              VgaCurrentMode.memory    = (unsigned short *)0x000B8000;
              VgaCurrentMode.text_mode = true;
              VgaWriteFont(Vga8x8_Font, 8);
	          ConsoleInitCursor();
              break;
         /*case VGA_90x30_TEXT:        // Bochs don´t recognize
              VgaWriteRegs(Vga90x30_Text);
              VgaCurrentMode.mode      = VGA_90x30_TEXT;
              VgaCurrentMode.width     = 90;
              VgaCurrentMode.height    = 30;
              VgaCurrentMode.memory    = (unsigned short *)0x000B8000;
              VgaCurrentMode.text_mode = true;
              VgaWriteFont(Vga8x8_Font, 8);
	          ConsoleInitCursor();
              break;
         case VGA_90x60_TEXT:          // Virtual PC and Real PC gives error
              VgaWriteRegs(Vga90x60_Text);
              VgaCurrentMode.mode      = VGA_90x60_TEXT;
              VgaCurrentMode.width     = 90;
              VgaCurrentMode.height    = 60;
              VgaCurrentMode.memory    = (unsigned short *)0x000B8000;
              VgaCurrentMode.text_mode = true;
              VgaWriteFont(Vga8x8_Font, 8);
	          ConsoleInitCursor();
              break;*/
         case VGA_320x200x256: // 320x200 256 colours	  
              VgaWriteRegs(Vga320x200x256);
			  VgaCurrentMode.mode      = VGA_320x200x256;
			  VgaCurrentMode.width     = 320;
			  VgaCurrentMode.height    = 200;
			  VgaCurrentMode.memory    = (unsigned short *)0x000A0000;
			  VgaCurrentMode.text_mode = false;
		      VgaDrawPixel = VgaDrawPixel8;
			  break;
         default:
              return -EINVAL; // Invalid Mode
    }
    return ESUCCESS;
}
