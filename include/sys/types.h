/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\sys\types.h
    @brief Vars definitions
*/
#ifndef INCLUDE_SYS_TYPES_H
#define INCLUDE_SYS_TYPES_H

/** @brief Null value (0) */
#define NULL    ((void *)0)

/** @brief Define Bool variables */
#define __BOOL_TRUE_FALSE_DEFINED
#ifndef __cplusplus
    #ifdef _Bool   // Bool é uma keyword no C++
         #undef _Bool
    #endif
    #ifndef __BOOL_TRUE_FALSE_DEFINED
	typedef char _Bool;
    #endif
    /** @brief Define Bool var's */ 
    #define bool _Bool
    /** @brief Bool var: false */ 
    #define false 0
    /** @brief Bool var: true */ 
    #define true 1
#endif

#endif
