/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\memset.c
    @brief Memory and string functions
*/

void *memset(void *dst, int val, unsigned count)
{
    unsigned char *temp = (unsigned char *)dst;
    for( ; count != 0; count--)
         *temp++ = val;
    return dst;
}
