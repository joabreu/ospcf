/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strcpy.c
    @brief Memory and string functions
*/

char *strcpy(char *to, const char *from)
{
    char *save = to;
    for (; (*to = *from); ++from, ++to)
         ;
    return save;
}
