/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\limits.h
    @brief Vars values
*/
#ifndef INCLUDE_LIB_LIMITS_H
#define INCLUDE_LIB_LIMITS_H

/** @brief Number of bits in a char */
#define CHAR_BIT 8
/** @brief Maximum value of type char  */
#define CHAR_MAX 127
/** @brief Minimum value of type char */
#define CHAR_MIN (-128)
/** @brief Maximum value of type signed char */
#define SCHAR_MAX 127
/** @brief Minimum value of type signed char */
#define SCHAR_MIN (-128)
/** @brief Maximum value of type unsigned char */
#define UCHAR_MAX 255
/** @brief Maximum value of type short */
#define SHRT_MAX 32767
/** @brief Minimum value of type short */
#define SHRT_MIN (-32768)
/** @brief Maximum value of type unsigned short */
#define USHRT_MAX 65535
/** @brief Maximum value of type int */
#define INT_MAX 2147483647
/** @brief Minimum value of type int */
#define INT_MIN (-2147483647-1)
/** @brief Maximum value of type unsigned int */
#define UINT_MAX 4294967295U
/** @brief Maximum value of type long */
#define LONG_MAX 2147483647L
/** @brief Minimum value of type long */
#define LONG_MIN (-2147483647L-1L)
/** @brief Maximum value of type unsigned long */
#define ULONG_MAX 4294967295UL

#endif
