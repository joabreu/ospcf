/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\atan2.c
    @brief Mathematical Functions
*/

double atan2(double __y, double __x)
{
    register double __value;
    __asm __volatile__
         ("fpatan\n\t"
          "fld %%st(0)"
          : "=t" (__value) : "0" (__x), "u" (__y));
    return __value;
}
