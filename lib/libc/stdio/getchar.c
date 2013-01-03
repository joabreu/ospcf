/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\getchar.c
    @brief Standard input/output library
*/
#include <stdio.h>
#include <os.h>

int getchar()
{
    int c = 0;
    while(c == 0)
         read(STDIN_FD, (char *)&c, 1);
    return c;
}
