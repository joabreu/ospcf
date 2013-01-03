/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\drivers\video\vga.h
    @brief VGA video driver
*/
#ifndef INCLUDE_SYSTEM_DRIVERS_VIDEO_VGA_H
#define INCLUDE_SYSTEM_DRIVERS_VIDEO_VGA_H

#include <sys\types.h>

/** @brief Structure of a VGA mode */
typedef struct VgaModeStruct
{
    /** @brief TRUE if this mode is supported by BIOS */
    bool text_mode;
    /** @brief VGA mode number ( See VGA_YYxYY_MODE ) */
    int mode;
    /** @brief Mode width. Number of vertical lines */
    int width;
    /** @brief Mode height. Number of horizontal lines */
    int height;
    /** @brief VGA Physical address to write data */
    unsigned short *memory;
} VgaModeStruct;

/** @brief Current VGA mode structure */
extern VgaModeStruct VgaCurrentMode;

/** @brief VGA port address: AC Index */
#define	VGA_AC_INDEX		0x3C0
/** @brief VGA port address: AC Write */
#define	VGA_AC_WRITE		0x3C0
/** @brief VGA port address: AC Read */
#define	VGA_AC_READ		0x3C1
/** @brief VGA port address: MISC write */
#define	VGA_MISC_WRITE		0x3C2
/** @brief VGA port address: SEQ index */
#define VGA_SEQ_INDEX		0x3C4
/** @brief VGA port address: SEQ data */
#define VGA_SEQ_DATA		0x3C5
/** @brief VGA port address: DAC read index */
#define	VGA_DAC_READ_INDEX	0x3C7
/** @brief VGA port address: DAC write index */
#define	VGA_DAC_WRITE_INDEX	0x3C8
/** @brief VGA port address: DAC data */
#define	VGA_DAC_DATA		0x3C9
/** @brief VGA port address: MISC read */
#define	VGA_MISC_READ		0x3CC
/** @brief VGA port address: GC index */
#define VGA_GC_INDEX 		0x3CE
/** @brief VGA port address: GC data */
#define VGA_GC_DATA 		0x3CF
/** @brief VGA port address: CRTC index */
#define VGA_CRTC_INDEX		0x3D4
/** @brief VGA port address: CRTC data */
#define VGA_CRTC_DATA		0x3D5
/** @brief VGA port address: INSTAT read */
#define	VGA_INSTAT_READ		0x3DA

/** @brief Number Seq registers */
#define	VGA_NUM_SEQ_REGS	5
/** @brief Number Crtc registers */
#define	VGA_NUM_CRTC_REGS	25
/** @brief Number GC registers */
#define	VGA_NUM_GC_REGS		9
/** @brief Number AC registers */
#define	VGA_NUM_AC_REGS		21
/** @brief Total Number of VGA registers */
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				                 VGA_NUM_GC_REGS + VGA_NUM_AC_REGS


/** @brief VGA 80x25 Text Mode */
#define VGA_80x25_TEXT        2
/** @brief VGA 80x50 Text Mode */
#define VGA_80x50_TEXT        3  // Standard
/** @brief VGA 90x30 Text Mode */
#define VGA_90x30_TEXT        4
/** @brief VGA 90x60 Text Mode */
#define VGA_90x60_TEXT        5
/** @brief VGA 320x200x256 Mode */
#define VGA_320x200x256       9

/** @brief VGA 40x25 Text Mode Registers */
extern unsigned char Vga40x25_Text[]; // 0
/** @brief VGA 40x50 Text Mode Registers */
extern unsigned char Vga40x50_Text[]; // 1
/** @brief VGA 80x25 Text Mode Registers */
extern unsigned char Vga80x25_Text[]; // 2
/** @brief VGA 80x50 Text Mode Registers */
extern unsigned char Vga80x50_Text[]; // 3
/** @brief VGA 90x30 Text Mode Registers */
extern unsigned char Vga90x30_Text[]; // 4
/** @brief VGA 90x60 Text Mode Registers */
extern unsigned char Vga90x60_Text[]; // 5
/** @brief VGA 640x480x2 Mode Registers */
extern unsigned char Vga640x480x2[];  // 6
/** @brief VGA 320x200x4 Mode Registers */
extern unsigned char Vga320x200x4[];  // 7
/** @brief VGA 640x480x16 Mode Registers */
extern unsigned char Vga640x480x16[]; // 8
/** @brief VGA 320x200x256 Mode Registers */
extern unsigned char Vga320x200x256[];       // 9
/** @brief VGA 320x200x256 ModeX Registers */
extern unsigned char Vga320x200x256_Modex[]; // 10
/** @brief VGA 8x8 Font Registers used in modes: 80x50, 90x30, 90x60*/
extern unsigned char Vga8x8_Font[2048];      // 11
/** @brief VGA 8x16 Font Registers used in modes: 80x25 */
extern unsigned char Vga8x16_Font[4096];     // 12

/** @fn void VgaDrawCircleFill(int mX, int mY, int radius, int c)
    @brief Draws a filled circle
    @ingroup sys_drivers_video_vga
    @param mX X position of circle's centre
    @param mY Y position of circle's centre
    @param radius Circle radius
    @param c Circle colour
*/
void VgaDrawCircleFill(int mX, int mY, int radius, int c);
/** @fn void VgaDrawCircle(int mX, int mY, int radius, int c)
    @brief Draws a circle
    @ingroup sys_drivers_video_vga
    @param mX X position of circle's centre
    @param mY Y position of circle's centre
    @param radius Circle radius
    @param c Circle colour
*/
void VgaDrawCircle(int mX, int mY, int radius, int c);
/** @fn void VgaDrawRectangleFill(int left,int top, int right, int bottom, int c)
    @brief Draws a filled rectangle. See VgaDrawRectangle()
    @ingroup sys_drivers_video_vga
    @param left Position of left side
    @param top Position of top side
    @param right Position of right side
    @param bottom Position of bottom side
    @param c Rectangle colour
*/
void VgaDrawRectangleFill(int left,int top, int right, int bottom, int c);
/** @fn void VgaDrawVLine(int x,int y1, int y2, int c)
    @brief Draw a vertical line from (x, y1) to (x, y2)
    @ingroup sys_drivers_video_vga
    @param x X position
    @param y1 Y origin
    @param y2 Y end
    @param c Line colour
*/
void VgaDrawVLine(int x,int y1, int y2, int c);
/** @fn void VgaDrawHLine(int x1,int x2, int y, int c)
    @brief Draw a horizontal line from (x1, y) to (x2, y)
    @ingroup sys_drivers_video_vga
    @param x1 X origin
    @param x2 X end
    @param y Y position
    @param c Line colour
*/
void VgaDrawHLine(int x1,int x2, int y, int c);
/** @fn void VgaDrawLine(int x1, int y1, int x2, int y2, unsigned char c)
    @brief Draw a line from (x1, y1) to (x2, y2)
    @ingroup sys_drivers_video_vga
    @param x1 X origin
    @param y1 Y origin
    @param x2 X end
    @param y2 Y end
    @param c Line colour
*/
void VgaDrawLine(int x1, int y1, int x2, int y2, unsigned char c);
/** @fn void VgaDrawRectangle(int left, int top, int right, int bottom, int c)
    @brief Draws a rectangle
    
    left - Distance of left side from x=0
    top - Distance of top side from y=0
    right - Distance of right side from x=0
    bottom - Distance of bottom side from y=0
    Ex.: VgaDrawRectangle(10, 10, 50, 50, RED);
    (0,0)  :      (10, 0)                       (50,0)
           |
    (0, 10)|      (10,10)-----------------------(50,10)
           |         |                             |
           |         |                             |
           |         |                             |
           |         |                             |
    (0, 50)|      (10,50)-----------------------(50,50)
    @ingroup sys_drivers_video_vga
    @param left Position of left side
    @param top Position of top side
    @param right Position of right side
    @param bottom Position of bottom side
    @param c Rectangle colour
*/
void VgaDrawRectangle(int left, int top, int right, int bottom, int c);
/** @fn void VgaSetPlane(unsigned p)
    @brief VGA set plane
    @ingroup sys_drivers_video_vga
    @param p Plane
*/
void VgaSetPlane(unsigned p);
/** @fn void VgaWriteFont(unsigned char *buf, unsigned font_height)
    @brief Writes a font to VGA driver
    @ingroup sys_drivers_video_vga
    @param buf Font location
    @param font_height Font height
*/
void VgaWriteFont(unsigned char *buf, unsigned font_height);
/** @fn int VgaSavePalette(int *palette)
    @brief Saves current palette
    @ingroup sys_drivers_video_vga
    @param palette Data destination
    @return TRUE
*/
int VgaSavePalette(int *palette);
/** @fn int VgaSetPalette(int *palette)
    @brief Sets current VGA palette
    @ingroup sys_drivers_video_vga
    @param palette Data origin
    @return TRUE
*/
int VgaSetPalette(int *palette);
/** @brief Sets pixel (x,y) with colour c */
void (*VgaDrawPixel)(unsigned x, unsigned y, unsigned c);
/** @fn void VgaReadRegs(unsigned char *regs)
    @brief Reads VGA registers
    @ingroup sys_drivers_video_vga
    @param regs Data destination
*/
void VgaReadRegs(unsigned char *regs);
/** @fn void VgaWriteRegs(unsigned char *regs)
    @brief Writes VGA registers
    @ingroup sys_drivers_video_vga
    @param regs Data origin
*/
void VgaWriteRegs(unsigned char *regs);
/** @fn int VgaSetMode(int mode)
    @brief Sets current VGA mode
    @ingroup sys_drivers_video_vga
    @param mode Mode number. See defines in vga.h
    @return Error code
*/
int VgaSetMode(int mode);
/** @fn unsigned int VgaGetSeg()
    @brief Returns VGA current segment
    @ingroup sys_drivers_video_vga
    @return VGA screen location
*/
unsigned int VgaGetSeg();
/** @fn void VgaDelay()
    @brief Simple for loop
    @ingroup sys_drivers_video_vga
*/
void VgaDelay();
/** @fn void VgaInit()
    @brief Inits VGA driver. Set selected mode
    @ingroup sys_drivers_video_vga
*/
void VgaInit();
/** @fn void VgaDrawX()
    @brief Draw a 2 colour X into the screen. 320x200 mode.
    @ingroup sys_drivers_video_vga
*/
void VgaDrawX();
/** @fn void VgaDemo()
    @brief Use some simple VGA functions. 320x200 mode. For internal shell use
    @ingroup sys_drivers_video_vga
*/
void VgaDemo();

#endif
