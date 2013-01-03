/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\puts.c
    @brief Standard input/output library
*/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <os.h>

int puts(const char *str)
{
    int ret = write(STDOUT_FD, (char *)str, strlen(str));
    int ret2 = write(STDOUT_FD, (char *)"\n", 1);
    if((ret == strlen(str)) && (ret2 == 1))
         return ESUCCESS;
    else
         return EOF;
}
