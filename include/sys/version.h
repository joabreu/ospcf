/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\sys\version.h
    @brief DJGPP requirement
*/
/* DJGPP Version 2.3 */
#undef DJGPP
#undef __DJGPP
#undef __DJGPP__

#define DJGPP 2
#define __DJGPP 2
#define __DJGPP__ 2

#undef DJGPP_MINOR
#undef __DJGPP_MINOR
#undef __DJGPP_MINOR__

#define DJGPP_MINOR 3
#define __DJGPP_MINOR 3
#define __DJGPP_MINOR__ 3
