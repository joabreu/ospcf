/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\cosh.c
    @brief Mathematical Functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <math.h>

double cosh(double x)
{
    const double ebig = exp(fabs(x));
    return (ebig + 1.0/ebig) / 2.0;
}
