/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\printf.c
    @brief Standard input/output library
*/
#include <stdarg.h>
#include <stdio.h>

/** @fn static int printf_helper(unsigned c, void **data)
    @brief Helper for printf function. Prints a single char
    @ingroup libs_libc_stdio
    @param c Char
    @param data Unused
    @return 0
    @internal
*/
static int printf_helper(unsigned c, void **data)
{
    // Output single char in stdout
    putchar(c);
    return 0;
}
int printf(const char *fmt, ...)
{
    int ret = 0;
    va_list args;
    
    va_start(args, fmt);
    ret = do_printf(fmt, args, printf_helper, 0);
    va_end(args);
    
    return ret;
}
/*
int printf(char *format)
{
    int i = strlen(format);
	int j = 0;

    while((format[j] != '\0') && (j!=i))
    {
         putchar(format[j++]);
    }
	return j;
}
*/
