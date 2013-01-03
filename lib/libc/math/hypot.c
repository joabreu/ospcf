/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\math\hypot.c
    @brief Mathematical Functions
*/
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
/*
 * hypot() function for DJGPP.
 *
 * hypot() computes sqrt(x^2 + y^2).  The problem with the obvious
 * naive implementation is that it might fail for very large or
 * very small arguments.  For instance, for large x or y the result
 * might overflow even if the value of the function should not,
 * because squaring a large number might trigger an overflow.  For
 * very small numbers, their square might underflow and will be
 * silently replaced by zero; this won't cause an exception, but might
 * have an adverse effect on the accuracy of the result.
 *
 * This implementation tries to avoid the above pitfals, without
 * inflicting too much of a performance hit.
 *
 */
#include <float.h>
#include <math.h>
 
/* Approximate square roots of DBL_MAX and DBL_MIN.  Numbers
   between these two shouldn't neither overflow nor underflow
   when squared.  */
/** @brief Aproximate square root of DBL_MAX */
#define __SQRT_DBL_MAX 1.3e+154
/** @brief Aproximate square root of DBL_MIN */
#define __SQRT_DBL_MIN 2.3e-162

double _hypot(double x, double y)
{
    double abig = fabs(x), asmall = fabs(y);
    double ratio;
 
    /* Make abig = max(|x|, |y|), asmall = min(|x|, |y|).  */
    if (abig < asmall) {
         double temp = abig;
         abig = asmall;
         asmall = temp;
    }
    /* Trivial case.  */
    if (asmall == 0.)
         return abig;
 
    /* Scale the numbers as much as possible by using its ratio.
     For example, if both ABIG and ASMALL are VERY small, then
     X^2 + Y^2 might be VERY inaccurate due to loss of
     significant digits.  Dividing ASMALL by ABIG scales them
     to a certain degree, so that accuracy is better.  */
    if ((ratio = asmall / abig) > __SQRT_DBL_MIN && abig < __SQRT_DBL_MAX)
         return abig * sqrt(1.0 + ratio*ratio);
    else {
         /* Slower but safer algorithm due to Moler and Morrison.  Never
         produces any intermediate result greater than roughly the
         larger of X and Y.  Should converge to machine-precision
         accuracy in 3 iterations.  */
         double r = ratio*ratio, t, s, p = abig, q = asmall;
         do {
              t = 4. + r;
              if (t == 4.)
                   break;
              s = r / t;
              p += 2. * s * p;
              q *= s;
              r = (q / p) * (q / p);
         } while (1);
         return p;
    }
}
