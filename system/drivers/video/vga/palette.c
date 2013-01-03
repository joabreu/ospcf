/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\palette.c
    @brief VGA driver
*/
#include <kernel\x86\io.h>
#include <drivers\video\vga.h>

/** @brief VGA Default Palette */
int VgaDefaultPalette[16*3];

int VgaSavePalette(int *palette)
{
    int i=0;
    outportb(VGA_DAC_READ_INDEX,0);  // tell the VGA that palette data is coming.
    for(i=0;i<16*3;i++)
    {
         palette[i] = inportb(VGA_DAC_DATA);    	
	}
    return 1;
}
int VgaSetPalette(int *palette)
{
    int i=0;

    outportb(VGA_DAC_WRITE_INDEX,0); // tell the VGA that palette data is coming.
    for(i=0;i<16*3;i++)
    {
         outportb(VGA_DAC_DATA,palette[i]);    	
	}	
    return 1;
}
