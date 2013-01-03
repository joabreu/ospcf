/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\fs\write.c
    @brief Virtual File System
*/
#include <errno.h>
#include <stdio.h>
#include <kernel\console.h>

int VfsWrite(int fd, char *buffer, unsigned int len)
{
    if(fd == STDOUT_FD) {
         unsigned int i = len;
         while(i > 0) {
              char tmp = *buffer;
              if (tmp == 0)
                   return len-i;
              ConsolePutch(tmp);
              i--;
              buffer++;
         }
         return len;
    }
    return -ENOTSUP;
}
