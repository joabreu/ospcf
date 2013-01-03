/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\video\console.c
    @brief Screen manipulation
*/
#include <kernel\console.h>
#include <kernel\x86\io.h>
#include <drivers\video\vga.h>
#include <drivers\char\keyboard.h>

/** @brief Current console */
ConsoleStruct ConsoleCurrent;
/** @brief Save special key value to return in next getchar() call */
static unsigned int StupidKeySave = 0;

void ConsoleInit()
{
    ConsoleCurrent.active = true;    // Virtual Console activa
    ConsoleCurrent.xpos = 0;         // Posição do cursor em X
    ConsoleCurrent.ypos = 0;         // Posição do cursor em Y
    ConsoleCurrent.colour = DEFAULT_COLOUR;// Cor predefinida
    unsigned int j = 0;         
    while(j < BUFFER_SIZE) {         // Limpa o Buffer do teclado
         ConsoleCurrent.data.buffer[j] = 0;
         j++;
    }
    ConsoleCurrent.data.inbytes = 0; // Dados que entraram
    ConsoleCurrent.data.readptr = 0; // Posição da leitura
    ConsoleCurrent.data.writeptr = 0;// Posição da escrita
    ConsoleCurrent.screen = (unsigned short *)0xB8000;   // Localização da memória do ecrã
    ConsoleClearScreen();
}
void ConsoleClearScreen()
{
    int i = 0;
    for (i = 0; i < VgaCurrentMode.width*VgaCurrentMode.height; i++)
         VgaCurrentMode.memory[i] = 0x20 | (ConsoleCalcAttrib(DEFAULT_COLOUR/* this defines the cursor colour */, COLOUR_BLACK) << 8);
         
    ConsoleCurrent.xpos = 0;
    ConsoleCurrent.ypos = 0;
    ConsoleSetCursor(ConsoleCurrent.xpos, ConsoleCurrent.ypos);
}
void ConsoleInput(const unsigned int value)
{
	if(value == 0)
		return; // Stupid
    if(ConsoleCurrent.data.inbytes < BUFFER_SIZE) {
         ConsoleCurrent.data.buffer[ConsoleCurrent.data.writeptr] = value;
         ConsoleCurrent.data.inbytes++;
         ConsoleCurrent.data.writeptr = (ConsoleCurrent.data.writeptr + 1) % BUFFER_SIZE;
    } else
         kprintf("\nERRO: Buffer do teclado cheio ...\n");
}
unsigned int ConsoleReadChar()
{
    unsigned int tmp;
    if(StupidKeySave) {
         tmp = StupidKeySave & ~KB_SPECIAL; // Not Special key bit set
         StupidKeySave = 0;
         return tmp;
    }
    if(ConsoleCurrent.data.inbytes) {
         // data available
         tmp = ConsoleCurrent.data.buffer[ConsoleCurrent.data.readptr];
         ConsoleCurrent.data.readptr = (ConsoleCurrent.data.readptr + 1) % BUFFER_SIZE;
         ConsoleCurrent.data.inbytes--;
         if(tmp & KB_SPECIAL) { //For special keys, the getch function first returns ASCII 0. The next call will then return special keys
              StupidKeySave = tmp; // Only for special keys !!!
              return 0xE0; // Extended
         }
         return tmp;
    } else
         return 0; // no data available
}
char ConsoleCalcAttrib(unsigned char text, unsigned char back)
{
    return (back << 4) | (text & 0x0F);
}
void ConsoleSetColour(unsigned char col)
{
    ConsoleCurrent.colour = col;
}
unsigned char ConsoleRetColour()
{
    return ConsoleCurrent.colour;
}
void ConsoleInitCursor()
{
    outportb(VGA_CRTC_INDEX, 0x0A);
    outportb(VGA_CRTC_DATA, 0);
	outportb(VGA_CRTC_INDEX, 0x0B);
	outportb(VGA_CRTC_DATA, 0);
}
void ConsoleSetCursor(int x, int y)
{
    unsigned short cursor = (y+1) * VgaCurrentMode.width + x;
    outportb(VGA_CRTC_INDEX, 14);         // Tell the VGA board we are setting the high cursor byte.
    outportb(VGA_CRTC_DATA, cursor >> 8); // Send the high cursor byte.
    outportb(VGA_CRTC_INDEX, 15);         // Tell the VGA board we are setting the low cursor byte.
    outportb(VGA_CRTC_DATA, cursor);      // Send the low cursor byte.
}
void ConsoleSetPos(int x, int y)
{
    if(x >= VgaCurrentMode.width)
         x = VgaCurrentMode.width-1;
    if(y >= VgaCurrentMode.height)
         y = VgaCurrentMode.height-1;
         
    ConsoleCurrent.xpos = x;
    ConsoleCurrent.ypos = y;
}
void ConsoleGetPos(int *x, int *y)
{
    *x = ConsoleCurrent.xpos;
    *y = ConsoleCurrent.ypos;
}
void ConsoleScroll()
{
    // Move the current text chunk that makes up the screen
    // back in the buffer by a line
    int i;
    for (i = 0; i < (VgaCurrentMode.height - 3)*VgaCurrentMode.width; i++) {
         VgaCurrentMode.memory[i] = VgaCurrentMode.memory[i+VgaCurrentMode.width];
    }
    // The last line should now be blank. Do this by writing
    // 80 spaces to it.
    for (i = (VgaCurrentMode.height - 3)*VgaCurrentMode.width; 
         i < (VgaCurrentMode.height - 2)*VgaCurrentMode.width; i++) {
         VgaCurrentMode.memory[i] = 0x20 | (ConsoleCurrent.colour << 8);
    }
    ConsoleCurrent.ypos = ConsoleCurrent.ypos - 1;//VgaCurrentMode.height - 3;
}
void ConsolePutch(char c)
{
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    unsigned short *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && ConsoleCurrent.xpos) {
         ConsoleCurrent.xpos--;
         c = ' ';
         location = VgaCurrentMode.memory + (ConsoleCurrent.ypos * VgaCurrentMode.width + ConsoleCurrent.xpos);
         *location = c /* Estabeleçe 'c' como o lower byte */| (ConsoleCurrent.colour << 8 );
                                                               /* Move os atributos 8 Bits para a esquerda */
                                                               /* Para os colocar na posição correcta */
    }
    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09) {
         ConsoleCurrent.xpos = (ConsoleCurrent.xpos+8) & ~(8-1);
    }
    // Handle carriage return
    else if (c == '\r') {
         ConsoleCurrent.xpos = 0;
    }
    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n') {
         ConsoleCurrent.xpos = 0;
         ConsoleCurrent.ypos++;
    }
    // Handle any other printable character.
    else if(c >= ' ') {
         location = VgaCurrentMode.memory + (ConsoleCurrent.ypos * VgaCurrentMode.width + ConsoleCurrent.xpos);
         *location = c | (ConsoleCurrent.colour << 8 );
         ConsoleCurrent.xpos++;
    }
    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (ConsoleCurrent.xpos >= VgaCurrentMode.width) {
         ConsoleCurrent.xpos = 0;
         ConsoleCurrent.ypos++;
    }
    // Scroll the screen if needed.
    if (ConsoleCurrent.ypos >= (VgaCurrentMode.height - 2)) { // This is counting with status bar
         ConsoleScroll();
    }
    // Move the hardware cursor.
    ConsoleSetCursor(ConsoleCurrent.xpos, ConsoleCurrent.ypos);
}
