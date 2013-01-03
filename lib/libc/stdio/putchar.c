/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\putchar.c
    @brief Standard input/output library
*/
#include <stdio.h>
#include <os.h>

int putchar(int c)
{
    //return fputc(c, stdout);
    return write(STDOUT_FD, (char *)&c, 1);
}
