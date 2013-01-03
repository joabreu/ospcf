/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\assert.h
    @brief Data debug
*/
#ifndef INCLUDE_LIB_ASSERT_H
#define INCLUDE_LIB_ASSERT_H

#ifdef NDEBUG // Debug NOT enabled

/** @brief The assert function is not enabled
    @param B Expression */
#define ASSERT(B) ((void)0)

#else // Debug is enabled

/** @fn void _assert(const char *expr, const char *file, unsigned int line)
    @brief Certifies this expression is true. Output debug error if not
    
    Internal error detection. (Ignored if NDEBUG is defined where <assert.h> is included). If expression equals zero, message printed on stderr and abort called to terminate execution
    @ingroup libs_libc_assert
    @param expr Expression to check
    @param file File that called this function
    @param line Code line that called this function
*/
void _assert(const char *expr, const char *file, unsigned int line);

/** @brief Checks it expression B is true. Output debug error if not
    @param B Expression */
#define ASSERT(B) ((B) ? (void)0 : _assert(#B, __FILE__, __LINE__))

#endif
#endif
