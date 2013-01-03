/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\float.h
    @brief Implementation-defined Floating-Point Limits
*/
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#ifndef INCLUDE_LIB_FLOAT_H
#define INCLUDE_LIB_FLOAT_H

/** @brief Float Epsilon */
extern float __dj_float_epsilon;
/** @brief Float Max */
extern float __dj_float_max;
/** @brief Float Min */
extern float __dj_float_min;

/** @brief Float Dig */
#define FLT_DIG		6
/** @brief Float Epsilon */
#define FLT_EPSILON	__dj_float_epsilon
/** @brief Float Mant Dig */
#define FLT_MANT_DIG	24
/** @brief Float Max */
#define FLT_MAX		__dj_float_max
/** @brief Float Max 10 Exp */
#define FLT_MAX_10_EXP	38
/** @brief Float Max Exp */
#define FLT_MAX_EXP	128
/** @brief Float Min */
#define FLT_MIN		__dj_float_min
/** @brief Float Min 10 Exp */
#define FLT_MIN_10_EXP	(-37)
/** @brief Float Min Exp */
#define FLT_MIN_EXP	(-125)
/** @brief Float Radix */
#define FLT_RADIX	2
/** @brief Float Rounds */
#define FLT_ROUNDS	1

/** @brief Double epsilon */
extern double __dj_double_epsilon;
/** @brief Double max */
extern double __dj_double_max;
/** @brief Double min */
extern double __dj_double_min;

/** @brief Double Dig */
#define DBL_DIG		15
/** @brief Double Epsilon */
#define DBL_EPSILON	__dj_double_epsilon
/** @brief Double Mant Dig */
#define DBL_MANT_DIG	53
/** @brief Double Max */
#define DBL_MAX		__dj_double_max
/** @brief Double Max 10 Exp */
#define DBL_MAX_10_EXP	308
/** @brief Double Max Exp */
#define DBL_MAX_EXP	1024
/** @brief Double Min */
#define DBL_MIN		__dj_double_min
/** @brief Double Min 10 Exp */
#define DBL_MIN_10_EXP	(-307)
/** @brief Double Min Exp */
#define DBL_MIN_EXP	(-1021)

/** @brief Long Double Epsilon */
extern long double __dj_long_double_epsilon;
/** @brief Long Double Max */
extern long double __dj_long_double_max;
/** @brief Long Double Min */
extern long double __dj_long_double_min;

/** @brief Long Double Dig */
#define LDBL_DIG	18
/** @brief Long Double Epsilon */
#define LDBL_EPSILON	__dj_long_double_epsilon
/** @brief Long Double Dig */
#define LDBL_MANT_DIG	64
/** @brief Long Double Max */
#define LDBL_MAX	__dj_long_double_max
/** @brief Long Double Max 10 Exp */
#define LDBL_MAX_10_EXP	4932
/** @brief Long Double Max Exp */
#define LDBL_MAX_EXP	16384
/** @brief Long Double Min */
#define LDBL_MIN	__dj_long_double_min
/** @brief Long Double Min 10 Exp */
#define LDBL_MIN_10_EXP	(-4931)
/** @brief Long Double Min Exp */
#define LDBL_MIN_EXP	(-16381)

/** @fn int _isnan(double __x)
    @brief is nan
    @ingroup libs_libc_float 
    @param __x Double
    @return Result
    @todo Complete
*/
int _isnan(double __x);
/** @fn int _isnanl(long double __x)
    @brief is nan long
    @ingroup libs_libc_float 
    @param __x Long Double
    @return Result
    @todo Complete
*/
int _isnanl(long double __x);
/** @fn int _isinf(double __x)
    @brief is inf
    @ingroup libs_libc_float 
    @param __x Double
    @return Result
    @todo Complete
*/
int _isinf(double __x);
/** @fn int _finite(double x)
    @brief finite
    @ingroup libs_libc_float 
    @param x Double
    @return Result
    @todo Complete
*/
int _finite(double x);
/** @fn int _isinfl(long double __x)
    @brief is inf long
    @ingroup libs_libc_float 
    @param __x Long Double
    @return Result
    @todo Complete
*/
int _isinfl(long double __x);

#endif
