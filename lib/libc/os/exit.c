/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\os\exit.c
    @brief OS specific functions
*/
#include <sys\syscall.h>

int exit(int errorcode)
{
    int ret = 0;
    SYSTEM_CALL_CORE(SYSTEMCALL_THREAD_EXIT, &errorcode, &ret);
    return ret;
}
