/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\putc.c
    @brief Standard input/output library
*/
#include <stdio.h>

int putc(int c, FILE *f)
{
    return fputc(c, f);
}
