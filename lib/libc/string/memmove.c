/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\memmove.c
    @brief Memory and string functions
*/

void *memmove(void *dst_ptr, const void *src_ptr, unsigned len)
{
    char *dst = dst_ptr;
    const char *src = src_ptr;
    if((src < dst) && (dst < (src + len))) {
         src += len;
         dst += len;
         while(len--)
              *--dst = *--src;
    }
    else
         while (len--)
              *dst++ = *src++;
    return dst_ptr;
}
