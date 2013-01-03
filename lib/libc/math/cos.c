/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\cos.c
    @brief Mathematical Functions
*/

double cos(double __x)
{
    register double __value;
    __asm __volatile__
         ("fcos"
          : "=t" (__value): "0" (__x));
    return __value;
}
