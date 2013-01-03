/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strlen.c
    @brief Memory and string functions
*/

unsigned strlen(const char *str)
{
    const char *start = str;
    while(*str)
         str++;
    return str - start;
}
