/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\sinh.c
    @brief Mathematical Functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <math.h>

double sinh(double x)
{
    if(x >= 0.0) {
         const double epos = exp(x);
         return (epos - 1.0/epos) / 2.0;
    } else {
         const double eneg = exp(-x);
         return (1.0/eneg - eneg) / 2.0;
    }
}
