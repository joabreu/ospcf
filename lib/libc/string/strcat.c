/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strcat.c
    @brief Memory and string functions
*/

char *strcat(char *str, const char *append)
{
    char *save = str;
    for (; *str; ++str)
         ;
    while ((*str++ = *append++))
         ;
    return save;
}
