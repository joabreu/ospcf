/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\errno\errno.c
    @brief Error Codes Reported by (Some) Library Functions
*/
/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <errno.h>

int *_geterrno()
{
    /** @todo Implement process error list */
    return (int *)0;
    //return &ThrGetThreadInfo()->status;
}
