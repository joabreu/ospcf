/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\video\kprintf.c
    @brief Kernel printf
*/
#include <stdio.h>
#include <stdarg.h>
#include <kernel\console.h>

/** @fn static int kprintfHelper(unsigned c, void **data)
    @brief Print a single char into the screen using ConsolePutch(...)
    @ingroup sys_kernel_video 
    @param c Char
    @param data Not used
    @return Zero
    @internal
*/
static int kprintfHelper(unsigned c, void **data)
{
	ConsolePutch(c);
    return 0;
}
int kprintf(const char *format, ...)
{
	int ret;
	va_list args; /** @todo Lock here */
	
	va_start(args, format);
	ret = do_printf(format, args, kprintfHelper, NULL);
	va_end(args); /** @todo Unlock here */
	return ret;
}
int kprintfc(unsigned char colour, const char *format, ...)
{
    int ret;
    va_list args; /** @todo Lock here */
    unsigned char old = ConsoleRetColour();
    
    ConsoleSetColour(colour);
    va_start(args, format);
    ret = do_printf(format, args, kprintfHelper, NULL);
    va_end(args);
    
    ConsoleSetColour(old); /** @todo Unlock here */
    return ret;
}
    
