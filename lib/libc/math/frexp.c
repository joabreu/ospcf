/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\frexp.c
    @brief Mathematical Functions
*/
#include <sys\types.h>
#include <libc\internal.h>
#include <math.h>

double frexp(double __x, int *exptr)
{
	double_t *x = (double_t *)&__x;
	
	if(exptr != NULL)
		 *exptr = x->exponent - 0x3FE;
		
	x->exponent = 0x3FE;
	return __x; 
}
