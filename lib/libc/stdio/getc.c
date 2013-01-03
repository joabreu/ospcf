/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\getc.c
    @brief Standard input/output library
*/
#include <stdio.h>

int getc(FILE *f)
{
    return fgetc(f);
}
