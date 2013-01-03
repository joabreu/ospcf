/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\math.h
    @brief Mathematical Functions
*/
/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1998 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#ifndef INCLUDE_LIB_MATH_H
#define INCLUDE_LIB_MATH_H

/** @brief Huge Value */
extern double __dj_huge_val;
/** @brief Huge Value */
#define HUGE_VAL  __dj_huge_val

/** @fn double sin(double __x)
    @brief __x sinus
    @ingroup libs_libc_math 
    @param __x Double
    @return Sinus
*/
double sin(double __x);
/** @fn double cos(double __x)
    @brief Co-sine of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Co-sine of __x
*/
double cos(double __x);
/** @fn double tan(double __x)
    @brief __x tangent
    @ingroup libs_libc_math 
    @param __x Double
    @return Result
*/
double tan(double __x);
/** @fn double sinh(double x)
    @brief Hyperbolic sine of x
    @ingroup libs_libc_math 
    @param x Double
    @return Result
*/
double sinh(double x);
/** @fn double cosh(double x)
    @brief Hyperbolic cosine of x
    @ingroup libs_libc_math 
    @param x Double
    @return Hyperbolic cosine of x
*/
double cosh(double x);
/** @fn double tanh(double x)
    @brief Hyperbolic tangent of x
    @ingroup libs_libc_math 
    @param x Double
    @return Result
*/
double tanh(double x);
/** @fn double asin(double __x)
    @brief Arc-sine of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Arc-sine of __x
*/
double asin(double __x);
/** @fn double acos(double __x)
    @brief Arc-cosine of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Arc-cosine of __x
*/
double acos(double __x);
/** @fn double atan(double __x)
    @brief Arc-tangent of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Arc-tangent of __x
*/
double atan(double __x);
/** @fn double atan2(double __y, double __x)
    @brief See atan(...)
    @ingroup libs_libc_math 
    @param __y Double
    @param __x Double
    @return Double
*/
double atan2(double __y, double __x);
/** @fn double exp(double __x)
    @brief Exponential of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Exponential of __x
*/
double exp(double __x);
/** @fn double log(double __x)
    @brief Natural logarithm of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Result
*/
double log(double __x);
/** @fn double log10(double __x)
    @brief Base-10 logarithm of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Result
*/
double log10(double __x);
/** @fn double pow(double __x, double __y)
    @brief __x raised to power __y
    @ingroup libs_libc_math 
    @param __x Double
    @param __y Double
    @return Result
*/
double pow(double __x, double __y);
/** @fn double sqrt(double __x)
    @brief __x square root
    @ingroup libs_libc_math 
    @param __x Double
    @return Result
*/
double sqrt(double __x);
/** @fn double ceil(double __x)
    @brief Smallest integer not less than __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Smallest integer not less than __x
*/
double ceil(double __x);
/** @fn double floor(double __x)
    @brief Largest integer not greater than __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Largest integer not greater than __x
*/
double floor(double __x);
/** @fn double fabs(double __x)
    @brief Absolute value of __x
    @ingroup libs_libc_math 
    @param __x Double
    @return Absolute value of __x
*/
double fabs(double __x);
/** @fn double ldexp(double __x, int __y)
    @brief __x times 2 to the power __y
    @ingroup libs_libc_math 
    @param __x Double
    @param __y Exponent
    @return Result
*/
double ldexp(double __x, int __y);
/** @fn double frexp(double __x, int *exptr)
    @brief frexp
    
    If __x non-zero, returns value, with absolute value in interval [1/2, 1), and assigns to *exptr integer such that product of return value and 2 raised to the power *exptr equals __x; if __x zero, both return value and *exptr are zero 
    @ingroup libs_libc_math 
    @param __x Double
    @param exptr Exponent Pointer
    @return Result
*/
double frexp(double __x, int *exptr);
/** @fn double modf(double __x, double *__i)
    @brief Returns fractional part and assigns to *__i integral part of __x, both with same sign as __x
    @ingroup libs_libc_math 
    @param __x Double
    @param __i Integral Part
    @return Fractional Part
*/
double modf(double __x, double *__i);
/** @fn double fmod(double __x, double __y)
    @brief Return remainder of floating-point division
    
    If __y non-zero, floating-point remainder of __x/__y, with same sign as __x; if __y zero, result is implementation-defined 
    @ingroup libs_libc_math 
    @param __x Double
    @param __y Double
    @return Remainder floating-point
*/
double fmod(double __x, double __y);

/** @fn double _hypot(double x, double y)
    @brief Hypot of x and y
    @ingroup libs_libc_math 
    @param x Double
    @param y Double
    @return Double
*/ 
double _hypot(double x, double y);

int _finite(double x);
int _isnan(double __x);
int _isinf(double __x);

int _isnanl(long double __x);
int _isinfl(long double __x);
/** @fn long double modfl(long double __x, long double *__i)
    @brief Returns fractional part and assigns to *__i integral part of __x, both with same sign as __x
    @ingroup libs_libc_math 
    @param __x Long Double
    @param __i Long Double
    @return Long Double
*/ 
long double modfl(long double __x, long double *__i);
/** @fn long double powl(long double __x,long double __y)
    @brief __x raised to power __y
    @ingroup libs_libc_math 
    @param __x Long Double
    @param __y Long Double
    @return Long Double
*/
long double powl(long double __x,long double __y);
/** @fn long double ceill(long double __x)
    @brief See ceil(...)
    @ingroup libs_libc_math 
    @param __x Long Double
    @return Long Double
*/
long double ceill(long double __x);
/** @fn long double floorl(long double x)
    @brief See floor(...)
    @ingroup libs_libc_math 
    @param x Long Double
    @return Long Double
*/
long double floorl(long double x);


/** @fn double acosh(double x)
    @brief See acos(...)
    @ingroup libs_libc_math 
    @param x Double
    @return Double
*/
double acosh(double x);
/** @fn double asinh(double x)
    @brief See asin(...)
    @ingroup libs_libc_math 
    @param x Double
    @return Double
*/
double asinh(double x);
/** @fn double atanh(double x)
    @brief See atan(...)
    @ingroup libs_libc_math 
    @param x Double
    @return Double
*/
double atanh(double x);


/** @fn double __log2(double __x)
    @brief See log(...)
    @ingroup libs_libc_math 
    @param __x Double
    @return Double
    @internal
*/ 
double __log2(double __x);

#endif
