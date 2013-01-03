/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\console.h
    @brief Screen manipulation
*/
#ifndef INCLUDE_SYSTEM_KERNEL_CONSOLE_H
#define INCLUDE_SYSTEM_KERNEL_CONSOLE_H

#include <sys\types.h>

/** @brief Maximum size of console buffer (bytes). The console buffer is used to store keyboard data */
#define BUFFER_SIZE          256

/** @brief Default colour of console */
#define DEFAULT_COLOUR    7

/** @brief Colour Black */
#define COLOUR_BLACK         0  // Preto
/** @brief Colour Blue */
#define COLOUR_BLUE          1  // Azul
/** @brief Colour Green */
#define COLOUR_GREEN         2  // Verde
/** @brief Colour Cyan */
#define COLOUR_CYAN          3  //
/** @brief Colour Red */
#define COLOUR_RED           4  // Vermelho
/** @brief Colour Magenta */
#define COLOUR_MAGENTA       5  // Magenta
/** @brief Colour Brown */
#define COLOUR_BROWN         6  // Castanho
/** @brief Colour Light Gray */
#define COLOUR_LIGHTGRAY     7  // Cinzento Claro
/** @brief Colour Dark Gray */
#define COLOUR_DARKGRAY      8  // Cinzento Escuro
/** @brief Colour Light Blue */
#define COLOUR_LIGHTBLUE     9  // Azul Claro
/** @brief Colour Light Green */
#define COLOUR_LIGHTGREEN    10 // Verde Claro
/** @brief Colour Light Cyan */
#define COLOUR_LIGHTCYAN	 11 //
/** @brief Colour Pink */ 
#define COLOUR_PINK		     12 // Rosa
/** @brief Colour Light Magenta */
#define COLOUR_LIGHTMAGENTA  13 // Magenta Claro
/** @brief Colour Yellow */
#define COLOUR_YELLOW		 14 // Amarelo
/** @brief Colour White */
#define COLOUR_WHITE		 15 // Branco

/** @brief Console Data struct */
struct ConsoleData
{
    /** @brief Console input buffer, usually keyboard inputs */
    unsigned int  buffer[BUFFER_SIZE]; // In bytes (keyboard)
    /** @brief Count of inputed bytes */
    unsigned int  inbytes;
    /** @brief Current reading pointer */
    unsigned int  readptr;
    /** @brief Current writing pointer */
    unsigned int  writeptr;
};
/** @brief Console Struct */
typedef struct    // Estrutura de uma consola de texto
{
    /** @brief Is this console currently active ? */
    bool          active;
    /** @brief Current cursor X position */
    unsigned int  xpos;
    /** @brief Current cursor Y position */
    unsigned int  ypos;
    /** @brief Console text colour */
    unsigned char colour;
    /** @brief Console internal data */
    struct ConsoleData data;
    /** @brief Pointer to console screen */
    unsigned short *screen;
    /** @todo Owned by wich process ? */
} ConsoleStruct;

/** @fn void ConsoleInit()
    @brief Init console manager
    @ingroup sys_kernel_video
*/
void ConsoleInit();
/** @fn void ConsoleClearScreen()
    @brief Clears current screen
    @ingroup sys_kernel_video
*/
void ConsoleClearScreen();
/** @fn void ConsoleInput(const unsigned int value)
    @brief Input char into console buffer
    @ingroup sys_kernel_video
    @param value Input char
*/
void ConsoleInput(const unsigned int value);
/** @fn unsigned int ConsoleReadChar()
    @brief Return char from console buffer
    @ingroup sys_kernel_video
    @return Char
*/
unsigned int ConsoleReadChar();
/** @fn char ConsoleCalcAttrib(unsigned char text, unsigned char back)
    @brief Calculate console's colour
    
    Screen Word Format
    Bit 15: If set the text will not blink.
    15 . . . . 12 | 11 . . . . 8 | 7 . . . . . . . . 0
     back colour  |  text colour |   char ascii value
    @ingroup sys_kernel_video
    @param text Text colour
    @param back Back colour
    @return Result
*/
char ConsoleCalcAttrib(unsigned char text, unsigned char back);
/** @fn void ConsoleSetColour(unsigned char col)
    @brief Set console colour. Use ConsoleCalcAttrib()
    @ingroup sys_kernel_video
    @param col Colour
*/
void ConsoleSetColour(unsigned char col);
/** @fn unsigned char ConsoleRetColour()
    @brief Return console colour
    @ingroup sys_kernel_video
    @return Colour
*/
unsigned char ConsoleRetColour();
/** @fn void ConsoleInitCursor()
    @brief Inits blinking cursor
    @ingroup sys_kernel_video
*/
void ConsoleInitCursor();
/** @fn void ConsoleSetCursor(int x, int y)
    @brief Set blinking cursor at (x,y)
    @ingroup sys_kernel_video
    @param x X position
    @param y Y position
*/
void ConsoleSetCursor(int x, int y);
/** @fn void ConsoleSetPos(int x, int y)
    @brief Set console writing position
    @ingroup sys_kernel_video
    @param x X position
    @param y Y position
*/
void ConsoleSetPos(int x, int y);
/** @fn void ConsoleGetPos(int *x, int *y)
    @brief Get console writing position
    @ingroup sys_kernel_video
    @param x X position
    @param y Y position
*/
void ConsoleGetPos(int *x, int *y);
/** @fn void ConsoleScroll()
    @brief Scroll the screen up one line. WARNING: The last line is not scrolled because its reserved for status bar
    @ingroup sys_kernel_video
*/
void ConsoleScroll();
/** @fn void ConsolePutch(char c)
    @brief Puts a single char into the screen
    @ingroup sys_kernel_video
    @param c Char
*/
void ConsolePutch(char c);
/** @fn int kprintf(const char *format, ...)
    @brief Prints a formated string into the screen
    @ingroup sys_kernel_video
    @param format String
    @return Parsed string lenght
*/
int kprintf(const char *format, ...);
/** @fn int kprintfc(unsigned char colour, const char *format, ...)
    @brief Prints a formated string into the screen, select desired colour
    @ingroup sys_kernel_video
    @param colour String colour
    @param format String
    @return Parsed string lenght
*/
int kprintfc(unsigned char colour, const char *format, ...);

#endif
