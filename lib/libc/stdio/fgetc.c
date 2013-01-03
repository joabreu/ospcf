/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file lib\libc\stdio\fgetc.c
    @brief Standard input/output library
*/
#include <stdio.h>
#include <os.h>

int fgetc(FILE *f)
{
    int c = 0;
    
    // check flags (is readable ?)
    
    // check end of file
    
    int len = read(f->fd, (char *)&c, 1);
    if(len == 0)
         return EOF; // set flags as EOF
    else if(len < 0)
         return EOF; // set flags as error
    
    /*if(f->bufpos >= f->bufsize)
    {
         // Read some bytes in advance
         int len = read(f->fd, f->buffer, f->buflen);
         if(len == 0)
              return EOF; // set flags
         else if(len < 0) // set flags
              return EOF;
              
         f->bufsize = len;
         f->bufpos = 0;
    }
    c = f->buffer[f->bufpos++];*/
    return c;
}
