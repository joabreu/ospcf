/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strncmp.c
    @brief Memory and string functions
*/

int strncmp(const char *s1, const char *s2, unsigned n)
{
    if(n == 0)
         return 0;
    while(n-- != 0 && *s1 == *s2) {
         if(n == 0 || *s1 == '\0' || *s2 == '\0')
	          break;
         s1++;
         s2++;
    }
    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}
