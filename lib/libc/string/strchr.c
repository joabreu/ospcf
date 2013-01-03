/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strchr.c
    @brief Memory and string functions
*/

char *strchr(const char *str, int c)
{
    int p = 0;
    while((str[p] != c) && (str[p] != '\0'))
         p++;
    if(str[p] == '\0')
         return 0;
    return (char *)&(str[p]);
}
