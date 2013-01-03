/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\fs\read.c
    @brief Virtual File System
*/
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <kernel\console.h>

int VfsRead(int fd, char *buffer, unsigned int len)
{
    if(fd == STDIN_FD) {
         unsigned int i = len;
         while(i > 0) {
              unsigned int key = ConsoleReadChar();
              if(key == 0) {
                   *buffer = '\0';
                   return len-i;
              }
              memcpy(buffer, &key, 1);
              i--;
              buffer++;
         }
         return len;
    }
    return -ENOTSUP;
}
