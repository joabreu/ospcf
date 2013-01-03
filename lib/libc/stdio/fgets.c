/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\fgets.c
    @brief Standard input/output library
*/
#include <errno.h>
#include <stdio.h>

char *fgets(char *str, int size, FILE *f)
{
    char *orig = str;
    int len = 0;
    
    for(len = size; len > 1; len--) {
         register char c = fgetc(f);
         if(c == EOF)
              break;
         *str++ = c;
         if(c == '\n')
              break;
    }
    if((len == size /* nothing was read */) /* check if EOF */)
         return NULL;
    *str = '\0';
    return orig;
}
