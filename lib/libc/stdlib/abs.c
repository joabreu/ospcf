/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdlib\abs.c
    @brief Standard library functions
*/

int abs(int j)
{
    return j<0 ? -j : j;
}
