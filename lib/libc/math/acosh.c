/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\acosh.c
    @brief Mathematical Functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <math.h>

double acosh(double x)
{
    return log(x + sqrt(x*x - 1));
}
