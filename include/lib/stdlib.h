/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\stdlib.h
    @brief Standard library
*/
#ifndef INCLUDE_LIB_STDLIB_H
#define INCLUDE_LIB_STDLIB_H

/** @brief O valor máximo que o rand() pode retornar, é o máximo que uma variável do tipo int pode guardar */
#define RAND_MAX       32767

/** @fn int atoi(const char *str)
    @brief Turns string into a numeric value
    @ingroup libs_libc_stdlib
    @param str String
    @return Int value of string
*/
int atoi(const char *str);
/** @fn int rand()
    @brief Returns a random number
    @ingroup libs_libc_stdlib
    @return Random int
*/
int rand();
/** @fn void srand(unsigned int seed)
    @brief Sets next maximum value ?
    @ingroup libs_libc_stdlib
    @param seed Maximum ?
*/
void srand(unsigned int seed);
/** @fn int abs(int j)
    @brief Returns the absolute value of a var
    @ingroup libs_libc_stdlib
    @param j Var
    @return Absolute value of j
*/
int abs(int j);

#endif
