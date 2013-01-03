/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\sys\float_dx.c
    @brief Library private functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <libc/internal.h>

/** @brief Double max */
double_t __dj_double_max     = { 0xffffffffU, 0xfffff, 0x7fe, 0x0 };
