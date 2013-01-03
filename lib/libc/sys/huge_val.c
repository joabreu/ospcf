/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\sys\huge_val.c
    @brief Library private functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <libc/internal.h>

/** @brief Huge Value */
double_t __dj_huge_val = { 0x00000, 0x00000, 0x7ff, 0x0 };
