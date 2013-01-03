/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\fputc.c
    @brief Standard input/output library
*/
#include <errno.h>
#include <stdio.h>
#include <os.h>

int fputc(int c, FILE *f)
{
    // check flags (if is writeable)    
    if(write(f->fd, (char *)&c, 1) != 1)
         return EOF; // set flags as EOF
    return ESUCCESS;
}
