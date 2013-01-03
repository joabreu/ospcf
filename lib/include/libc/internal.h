/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\include\libc\internal.h
    @brief Libc internal use
*/
#ifndef LIB_INCLUDE_LIBC_INTERNAL_H
#define LIB_INCLUDE_LIBC_INTERNAL_H

/** @brief Float type struct */
typedef struct
{
    /** @brief Mantissa */
    unsigned mantissa:23;
    /** @brief Exponent */
    unsigned exponent:8;
    /** @brief Sign */
    unsigned sign:1;
} float_t;
/** @brief Double type struct */
typedef struct
{
    /** @brief Mantissa low */
    unsigned mantissal:32;
    /** @brief Mantissa high */
    unsigned mantissah:20;
    /** @brief Exponent */
    unsigned exponent:11;
    /** @brief Sign */
    unsigned sign:1;
} double_t;
/** @brief Long Double type struct */
typedef struct
{
    /** @brief Mantissa low */
    unsigned mantissal:32;
    /** @brief Mantissa high */
    unsigned mantissah:32;
    /** @brief Exponent */ 
    unsigned exponent:15;
    /** @brief Sign */
    unsigned sign:1;
} long_double_t;

#endif
