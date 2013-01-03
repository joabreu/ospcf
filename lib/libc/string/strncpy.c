/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\strncpy.c
    @brief Memory and string functions
*/

char *strncpy(char *dst, const char *src, unsigned n)
{
    if (n != 0) {
         char *d = dst;
         const char *s = src;
         do {
              if ((*d++ = *s++) == 0) {
	               while(--n != 0)
	                    *d++ = 0;
                   break;
              }
         } while(--n != 0);
    }
    return dst;
}
