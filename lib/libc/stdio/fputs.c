/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\fputs.c
    @brief Standard input/output library
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <os.h>

int fputs(const char *str, FILE *f)
{
    int ret = write(f->fd, (char *)str, strlen(str));
    int ret2 = write(f->fd, (char *)"\n", 1);
    if((ret == strlen(str)) && (ret2 == 1))
         return ESUCCESS;
    else
         return EOF;
}
