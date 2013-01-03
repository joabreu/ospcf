/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\os\write.c
    @brief OS specific functions
*/
#include <errno.h>
#include <sys\syscall.h>

int write(int fd, char *buffer, unsigned int len)
{
    int ret = 0;
    
    if((buffer == NULL) || (len == 0))
         return -EINVAL;
    
    SYSTEM_CALL_CORE(SYSTEMCALL_WRITE, &fd, &ret);
    return ret;
}
