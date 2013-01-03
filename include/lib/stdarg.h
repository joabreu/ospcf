/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\stdarg.h
    @brief Arguments manipulation
*/
#ifndef INCLUDE_LIB_STDARG_H
#define INCLUDE_LIB_STDARG_H

#include "va_list.h"

/** @brief Width of stack == width of int */
#define	STACKITEM	int

/* round up width of objects pushed on stack. The expression before the
& ensures that we get 0 for objects of size 0. */
/** @brief Returns the size of TYPE
    @param TYPE Return size of this
    @return Size
*/
#define	VA_SIZE(TYPE)					\
	((sizeof(TYPE) + sizeof(STACKITEM) - 1)	\
		& ~(sizeof(STACKITEM) - 1))

/* &(LASTARG) points to the LEFTMOST argument of the function call
(before the ...) */
/** @brief Points to first argument
    @param AP Arguments list
    @param LASTARG LEFTMOST arguments of the function call
*/
#define	va_start(AP, LASTARG)	\
	(AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

/** @brief Nothing
    @param AP Arguments list */
#define va_end(AP)	/* nothing */

/** @brief Get Next argument
    @param AP Arguments list
    @param TYPE Next argument type
*/
#define va_arg(AP, TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

#endif
