/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\atanh.c
    @brief Mathematical Functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <math.h>

double atanh(double x)
{
    return log((1+x)/(1-x)) / 2.0;
}
