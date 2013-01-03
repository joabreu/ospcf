/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\gets.c
    @brief Standard input/output library
*/
#include <stdio.h>

char *gets(char *str)
{
    int c;
    char *s;

    s = str;
    while (((c = getchar()) != '\n') && (c != EOF)) 
         *s++ = c;
    if((c == EOF) && (s == str)) // check flags
         return 0;
    *s++ = '\0';
    return s;
}
