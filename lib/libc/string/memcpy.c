/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\string\memcpy.c
    @brief Memory and string functions
*/

void *memcpy(void *dst_ptr, const void *src_ptr, unsigned len)
{
    void *ret_val = dst_ptr;
    const char *src = (const char *)src_ptr;
    char *dst = (char *)dst_ptr;
    for(; len != 0; len--)
         *dst++ = *src++;
    return ret_val;
}
