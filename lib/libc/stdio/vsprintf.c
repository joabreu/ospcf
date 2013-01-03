/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\vsprintf.c
    @brief Standard input/output library
*/
#include <stdarg.h>
#include <stdio.h>

/** @fn static int vsprintf_help(unsigned c, void **ptr)
    @brief Helper for vsprintf function. Prints a single char into a string
    @ingroup libs_libc_stdio
    @param c Char
    @param ptr Unused
    @return 0
    @internal
*/
static int vsprintf_help(unsigned c, void **ptr)
{
	char *dst;

	dst = *ptr;
	*dst++ = c;
	*ptr = dst;
	return 0 ;
}
int vsprintf(char *buffer, const char *fmt, va_list args)
{
	int ret_val;

	ret_val = do_printf(fmt, args, vsprintf_help, (void *)buffer);
	buffer[ret_val] = '\0';
	return ret_val;
}
