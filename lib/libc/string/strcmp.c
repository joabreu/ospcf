/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strcmp.c
    @brief Memory and string functions
*/

int strcmp(const char *s1, const char *s2)
{
    while(*s1 != '\0' && *s1 == *s2) {
         s1++;
         s2++;
    }
    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}
