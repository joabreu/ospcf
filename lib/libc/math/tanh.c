/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\tanh.c
    @brief Mathematical Functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <math.h>

double tanh(double x)
{
    if (x > 50)
         return 1;
    else if (x < -50)
         return -1;
    else {
         const double ebig = exp(x);
         const double esmall = 1.0/ebig;
         return (ebig - esmall) / (ebig + esmall);
    }
}
