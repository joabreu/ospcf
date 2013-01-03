/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\video\vga\draw.c
    @brief VGA driver
*/
#include <string.h>
#include <stdlib.h>
#include <drivers\video\vga.h>

/** @brief Returns the sign of a number
    @param x Number non-zero
    @return -1 or 0
*/
#define sgn(x) ((x<0)?-1:((x>0)?1:0))

void VgaDrawCircleFill(int mX, int mY, int radius, int c)
{
    int schmu=10000;
	int x,y,d;
	int i = 0;
	unsigned short dxoffset,dyoffset,offset=(mY<<8)+(mY<<6)+mX;
	unsigned short *VGA = (unsigned short *)VgaCurrentMode.memory;
	
	x=0;
	y=radius;
	d=(5*schmu/4)-(radius*schmu);
		 
    VgaDrawPixel( x+mX, y+mY,c);	
	VgaDrawPixel( y+mX, x+mY,c);
	VgaDrawPixel( y+mX,-x+mY,c);
	VgaDrawPixel( x+mX,-y+mY,c);
	
	VgaDrawPixel(-x+mX,-y+mY,c);
	VgaDrawPixel(-y+mX,-x+mY,c);
	VgaDrawPixel(-y+mX, x+mY,c);
	VgaDrawPixel(-x+mX, y+mY,c);
	
	while(y>=x)
    {
		 dxoffset = (x<<8) + (x<<6);
         dyoffset = (y<<8) + (y<<6);
         for(i=y;i>=x;i--,dyoffset-=VgaCurrentMode.width)
         {
              /*VgaDrawPixel( x+mX,   i+mY, c);
              VgaDrawPixel( y+mX,   x+mY, c);
              VgaDrawPixel( y+mX,  -x+mY, c);
              VgaDrawPixel( x+mX,  -i+mY, c);
              VgaDrawPixel(-x+mX,  -i+mY, c);
              VgaDrawPixel(-y+mX,  -x+mY, c);
              VgaDrawPixel(-y+mX,   x+mY, c);
              VgaDrawPixel(-x+mX,   i+mY, c);*/
              VGA[offset+i -dxoffset] = c;  // octant 0
              VGA[offset+x -dyoffset] = c;  // octant 1
              VGA[offset-x -dyoffset] = c;  // octant 2
              VGA[offset-i -dxoffset] = c;  // octant 3
              VGA[offset-i +dxoffset] = c;  // octant 4              
              VGA[offset-x +dyoffset] = c;  // octant 5
              VGA[offset+x +dyoffset] = c;  // octant 6
              VGA[offset+i +dxoffset] = c;  // octant 7
         }
		 if(d<0)
         {
			  d= d+(2*x +3)*schmu;
			  x++;
		 }else{
			  d = d+(2*(x-y)+5)*schmu;
			  x++;
			  y--;
		 }
	}
}
void VgaDrawCircle(int mX, int mY, int radius, int c)
{
    int schmu=10000;
	int x,y,d;
	
	x=0;
	y=radius;
	d=(5*schmu/4)-(radius*schmu);
		 
    VgaDrawPixel( x+mX, y+mY,c);	
	VgaDrawPixel( y+mX, x+mY,c);
	VgaDrawPixel( y+mX,-x+mY,c);
	VgaDrawPixel( x+mX,-y+mY,c);
	VgaDrawPixel(-x+mX,-y+mY,c);
	VgaDrawPixel(-y+mX,-x+mY,c);
	VgaDrawPixel(-y+mX, x+mY,c);
	VgaDrawPixel(-x+mX, y+mY,c);
	
	while(y>x)
    {
		 if(d<0)
         {
			  d= d+(2*x +3)*schmu;
			  x++;
		 }else{
			  d = d+(2*(x-y)+5)*schmu;
			  x++;
			  y--;
		 }
         VgaDrawPixel( x+mX, y+mY,c);	
		 VgaDrawPixel( y+mX, x+mY,c);
		 VgaDrawPixel( y+mX,-x+mY,c);
		 VgaDrawPixel( x+mX,-y+mY,c);
		 VgaDrawPixel(-x+mX,-y+mY,c);
		 VgaDrawPixel(-y+mX,-x+mY,c);
		 VgaDrawPixel(-y+mX, x+mY,c);
		 VgaDrawPixel(-x+mX, y+mY,c);
	}
}
void VgaDrawRectangleFill(int left,int top, int right, int bottom, int c)
{
    unsigned short top_offset,bottom_offset,i,temp,width;
    unsigned short *VGA = (unsigned short *)VgaCurrentMode.memory;
    
    if (top>bottom)
    {
         temp=top;
         top=bottom;
         bottom=temp;
    }
    if (left>right)
    {
         temp=left;
         left=right;
         right=temp;
    }
    top_offset=(top<<8)+(top<<6)+left;
    bottom_offset=(bottom<<8)+(bottom<<6)+left;
    width=right-left+1;
    for(i=top_offset;i<=bottom_offset;i+=VgaCurrentMode.width)
    {
         memset(&VGA[i],c,width);
    }
}
void VgaDrawVLine(int x,int y1, int y2, int c)
{
	int dy;
	for(dy=y1;dy<y2;dy++)
    {
	     VgaDrawPixel(x,dy,c);
 	}
}
void VgaDrawHLine(int x1,int x2, int y, int c)
{
	int dx;
	for(dx=x1;dx<x2;dx++)
    {
	     VgaDrawPixel(dx,y,c);
 	}
}
void VgaDrawLine(int x1, int y1, int x2, int y2, unsigned char c)
{
    int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

    dx=x2-x1;      // the horizontal distance of the line
    dy=y2-y1;      // the vertical distance of the line
    dxabs=abs(dx);
    dyabs=abs(dy);
    sdx=sgn(dx);
    sdy=sgn(dy);
    x=dyabs>>1;
    y=dxabs>>1;
    px=x1;
    py=y1;

    VgaDrawPixel(px,py,c);

    if (dxabs>=dyabs) // the line is more horizontal than vertical
    {
         for(i=0;i<dxabs;i++)
         {
              y+=dyabs;
              if (y>=dxabs)
              {
                   y-=dxabs;
                   py+=sdy;
              }
              px+=sdx;
              VgaDrawPixel(px,py,c);
         }
    }
    else // the line is more vertical than horizontal
    {
         for(i=0;i<dyabs;i++)
         {
              x+=dxabs;
              if (x>=dyabs)
              {
                   x-=dyabs;
                   px+=sdx;
              }
              py+=sdy;
              VgaDrawPixel(px,py,c);
         }
    }     
}
void VgaDrawRectangle(int left, int top, int right, int bottom, int c)
{
    VgaDrawLine(left,top,right,top,c);
    VgaDrawLine(left,top,left,bottom,c);
    VgaDrawLine(right,top,right,bottom,c);
    VgaDrawLine(left,bottom,right,bottom,c);
}
