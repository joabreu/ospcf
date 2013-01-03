/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strncat.c
    @brief Memory and string functions
*/

char *strncat(char *dst, const char *src, unsigned n)
{
    if (n != 0) {
         char *d = dst;
         const char *s = src;
         while(*d != 0)
              d++;
         do {
              if ((*d = *s++) == 0)
	               break;
              d++;
         } while(--n != 0);
         *d = 0;
    }
    return dst;
}
