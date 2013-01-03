/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strrchr.c
    @brief Memory and string functions
*/
#include <string.h>

char *strrchr(const char *s, int c)
{
    char *ptr = 0;
    // Check params
    if(s == 0)
         return ((char *)s);

    ptr = (char *)(s + strlen(s) - 1);

    while(ptr >= s) {
         if(ptr[0] == (char)c)
	          return (ptr);
         ptr -= 1;
    }
    return 0; // The string does not have a character = c
}
