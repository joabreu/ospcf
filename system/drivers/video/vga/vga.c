/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\vga.c
    @brief VGA driver
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <kernel\console.h>
#include <kernel\proc.h>
#include <kernel\mem\kheap.h>
#include <kernel\x86\io.h>
#include <drivers\video\vga.h>

VgaModeStruct VgaCurrentMode;

unsigned int VgaGetSeg()
{
	unsigned int seg;

	outportb(VGA_GC_INDEX, 6);
	seg = inportb(VGA_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	     case 0:
	     case 1:
		      seg = 0x000A0000;
		      break;
	     case 2:
		      seg = 0x000B0000;
		      break;
	     case 3:
		      seg = 0x000B8000;
		      break;
	}
	return seg;
}
void VgaDelay()
{
    int i;
    for(i=0;i<10;i++)
         ;
}
void VgaInit()
{
    VgaCurrentMode.mode    = 0;
    VgaCurrentMode.width   = 80;
    VgaCurrentMode.height  = 25;
    VgaCurrentMode.memory  = (unsigned short *)VgaGetSeg();//0xB8000;
    VgaSetMode(VGA_80x50_TEXT);
}
void VgaDrawX()
{
	unsigned y;

    // draw 2-color X
	for(y = 0; y < VgaCurrentMode.height; y++)
	{
		 VgaDrawPixel((VgaCurrentMode.width - VgaCurrentMode.height) / 2 + y, y, 1);
		 VgaDrawPixel((VgaCurrentMode.height + VgaCurrentMode.width) / 2 - y, y, 2);
	}
}
/** @fn static void VgaSetAndClean()
    @brief Sets mode 320x200x256 and cleans the screen
    @ingroup sys_drivers_video_vga 
    @internal
*/
static void VgaSetAndClean()
{
    if(VgaCurrentMode.mode != VGA_320x200x256) // Already set
         VgaSetMode(VGA_320x200x256);
    
    int y, x = 0;
    // 256-color
              
   	for(y = 0; y < VgaCurrentMode.height; y++) // Slow, slow, slow
		 for(x = 0; x < VgaCurrentMode.width; x++)
			  VgaDrawPixel(x, y, 0);
}
/** @fn static int Vga4Fun(int argc, char **argv)
    @brief Draws a cross that keeps jumping around the screen. JUST FOR FUN :)
    @ingroup sys_drivers_video_vga
    @param argc Argument count
    @param argv Argument list
    @return Error code
    @internal
*/
static int Vga4Fun(int argc, char **argv)
{
    VgaSetAndClean();

    /*
    Funny Funny
    int XPosition, YPosition = 0;
    int DeltaX, DeltaY = 0;
    int Delta = 1;
    int size = 10;

    unsigned int MaximumX = CurrentVgaMode.Width;
    unsigned int MaximumY = CurrentVgaMode.Height;
  
    DeltaX = Delta;
    DeltaY = Delta;
    
    VgaDrawRectangle(10,10,MaximumX-10,MaximumY-10,2);
  
    while(1)
    {
         XPosition+=DeltaX;
         YPosition+=DeltaY;

         if(XPosition < 0)
         {
              XPosition = 0;
              DeltaX = Delta;
         }
         else if((XPosition + size) > MaximumX)
         {
              XPosition = MaximumX - size;
              DeltaX = -Delta;
         }
         
         if(YPosition < 0)
         {
              YPosition = 0;
              DeltaY = Delta;
         }
         else if((YPosition + size) > MaximumY)
         {
              YPosition = MaximumY - size;
              DeltaY = -Delta;
         }
         VgaDrawCircleFill(XPosition, YPosition, 4, 2);
         delay(10);
         VgaDrawCircleFill(XPosition, YPosition, 4, 0); // Erase it
    }*/
    
    int xdelta, ydelta = 0;
    int delta = 1;
    int size = 4;
    int spacing = 10;
    int colour = COLOUR_GREEN;
    int xpos, ypos = spacing;
    
    unsigned int xmax = VgaCurrentMode.width-spacing;
    unsigned int ymax = VgaCurrentMode.height-spacing;
    
    xdelta = ydelta = delta;
    
    //VgaDrawRectangle(spacing-1, spacing-1, xmax+1, ymax+1, colour); // Ball jumping inside a rectangle
    VgaDrawRectangle(spacing, spacing, xmax, ymax, colour); // Ball erasing parts of a rectangle
    
    while(1)
    {
         xpos+=xdelta;
         ypos+=ydelta;
         
         if((xpos-size) <= spacing)
         {
              xpos = spacing + size;
              xdelta = delta;
         }
         else if((xpos + size) >= xmax)
         {
              xpos = xmax - size;
              xdelta = -delta;
         }
         
         if((ypos-size) <= spacing)
         {
              ypos = spacing + size;
              ydelta = delta;
         }
         else if((ypos + size) >= ymax)
         {
              ypos = ymax - size;
              ydelta = -delta;
         }
         VgaDrawCircleFill(xpos, ypos, 4, colour);
         //delay(10);
         VgaDrawCircleFill(xpos, ypos, 4, 0); // Erase it
    }
    return 0;
}
void VgaDemo()
{
    char key;
    int x, y, i, color = 0;
    int x2, y2 = 0;
    int radius = 0;
    int pid = 0;
    
    kprintf("AVISO :\n"
           "Controlador grafico do PCF ainda em fase BETA\n"
           "Prima uma tecla para continuar\n");
	unsigned char *screen = (unsigned char *)kmalloc(VgaCurrentMode.height * VgaCurrentMode.width * 2);
    
    // *2 varia com os modes
    // Warning ---------------------------------------------------------------------------|
    memcpy(screen, VgaCurrentMode.memory, (VgaCurrentMode.height * VgaCurrentMode.width * 2));    
    getchar();
	
    kprintf("\n\n-- Demo do motor grafico do PCF - Opcoes disponiveis --\n");
	kprintf("1: Dispor aleatoriamente 1000 pixeis no ecra(320x200x256)\n");
	kprintf("2: Desenhar 100 linhas aleatoriamente(320x200x256)\n");
	kprintf("3: Desenhar 100 circulos aleatoriamente(320x200x256)\n");
	kprintf("4: Desenhar 100 circulos(preenchidos) aleatoriamente(320x200x256)\n");
	kprintf("5: Desenhar 100 rectangulos aleatoriamente(320x200x256)\n");
	kprintf("6: Desenhar 100 rectangulos (preenchidos) aleatoriamente(320x200x256)\n");
	kprintf("7: Desenhar um Bitmap pre defenido(320x200x256)\n");
	kprintf("8: Desenhar um X no ecra(320x200x256)\n");
	kprintf("9: Vga 4 Fun\n");
	kprintf("\n0: Sair\n");
	
    while(1)
	{
         x = 0;
         y = 0;
         i = 0;
         color = 0;
         x2 = 0;
         y2 = 0;
         radius = 0;
         key = 0;
         
         key = getchar();
         // *2 varia com os modes
         // Warning ---------------------------------------------------------------------------|
         memcpy(screen, VgaCurrentMode.memory, (VgaCurrentMode.height * VgaCurrentMode.width * 2));   
         
         
         switch(key)
         {
              case '0': 
                   memcpy(VgaCurrentMode.memory, screen, (VgaCurrentMode.height * VgaCurrentMode.width * 2));
                   kfree(screen);
                   kprintf("\n!! FIM !!\n");
                   return;
                   break;
              case '1':
                   VgaSetAndClean();
                   for(i=0;i<1000;i++)
                   {
                        x=rand()%VgaCurrentMode.width;
                        y=rand()%VgaCurrentMode.height;
                        color=rand()%16;
                        VgaDrawPixel(x, y, color);
                   }
                   getchar();
                   break;
              case '2':
                   VgaSetAndClean();
                   for(i=0;i<100;i++)
                   {
                        x=rand()%VgaCurrentMode.width;
                        y=rand()%VgaCurrentMode.height;
                        x2=rand()%VgaCurrentMode.width;
                        y2=rand()%VgaCurrentMode.height;
                        color=rand()%16;
                        VgaDrawLine(x,y,x2,y2,color);
                   }
                   getchar();
                   break;
              case '3':
                   VgaSetAndClean();
                   for(i=0;i<100;i++)
                   {
                        radius=rand()%90+1;
                        x=rand()%(VgaCurrentMode.width-radius*2)+radius;
                        y=rand()%(VgaCurrentMode.height-radius*2)+radius;
                        color=rand()%16;
                        VgaDrawCircle(x,y,radius,color);
                   }
                   getchar();
                   break;
              case '4':
                   VgaSetAndClean();
                   for(i=0;i<100;i++)
                   {
                        radius=rand()%90+1;
                        x=rand()%(VgaCurrentMode.width-radius*2)+radius;
                        y=rand()%(VgaCurrentMode.height-radius*2)+radius;
                        color=rand()%16;
                        VgaDrawCircleFill(x,y,radius,color);
                   }
                   getchar();
                   break;
              case '5':
                   VgaSetAndClean();
                   for(i=0;i<100;i++)
                   {
                        x=rand()%VgaCurrentMode.width;
                        y=rand()%VgaCurrentMode.height;
                        x2=rand()%VgaCurrentMode.width;
                        y2=rand()%VgaCurrentMode.height;
                        color=rand()%16;
                        VgaDrawRectangle(x,y,x2,y2,color);
                   }
                   getchar();
                   break;
              case '6':
                   VgaSetAndClean();
                   for(i=0;i<100;i++)
                   {
                        x=rand()%VgaCurrentMode.width;
                        y=rand()%VgaCurrentMode.height;
                        x2=rand()%VgaCurrentMode.width;
                        y2=rand()%VgaCurrentMode.height;
                        color=rand()%16;
                        VgaDrawRectangleFill(x,y,x2,y2,color);
                   }
                   getchar();
                   break;
              case '7':
                   VgaSetAndClean();
                   // Bitmap
                   getchar();
                   break;
              case '8':
                   VgaSetAndClean();
                   VgaDrawX();
                   getchar();
                   break;
              case '9':
                   pid = ThreadCreate(GetCurrentProcess(), Vga4Fun, NULL);
                   getchar();
                   ThreadKill(GetThread(GetCurrentProcess(), pid));
                   break;
              default:continue;
         }

   	     VgaSetMode(VGA_80x50_TEXT);   
         memcpy(VgaCurrentMode.memory, screen, (VgaCurrentMode.height * VgaCurrentMode.width * 2));
        
    }
}
