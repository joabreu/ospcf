/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdlib\rand.c
    @brief Standard library functions
*/
/* Copyright (C) 1999 DJ Delorie, see COPYING_DJ.txt for details */
#include <stdlib.h>

/** @brief To help in the random process */
static unsigned long long next = 1;

int rand()
{
    /* This multiplier was obtained from Knuth, D.E., "The Art of
       Computer Programming," Vol 2, Seminumerical Algorithms, Third
       Edition, Addison-Wesley, 1998, p. 106 (line 26) & p. 108 */
    next = next * 6364136223846793005LL + 1;
    /* was: next = next * 0x5deece66dLL + 11; */
    return (int)((next >> 21) & RAND_MAX);
}
void srand(unsigned int seed)
{
    next = seed;
}
