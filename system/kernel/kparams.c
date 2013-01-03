/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\kparams.c
    @brief Kernel arguments parser
*/
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <kernel\status.h>

/** @fn static int ParseNumber(const char *string)
    @brief Like atoi() it parses a string to a number. It supports sufixes like 'k' and 'M'
    
    If a 'k' is found the number is multiplied by 1024. If a 'M' is found the number is multiplied by 1024*1024.
    When a 'k' or a 'M' is found the function returns the number that is before the char
    @ingroup sys_kernel
    @param string String
    @return Number
    @internal
*/
static int ParseNumber(const char *string)
{
    /* This function supports strings suffixes:
            'k' = kilo : multiplies the number per 1024
            'M' = Mega : multiplies the number per 1024*1024
    */
    int value;
    char buffer[256];
    int i;

    for(i = 0; i<strlen(string); i++)
    {
         if(string[i] == 'k' || string[i] == 'K')
         {
              strncpy(buffer, string, i);
              value = atoi(buffer)*1024;
              return value;
         }
         else if(string[i] == 'M')
         {
              strncpy(buffer, string, i);
              value = atoi(buffer)*1024*1024;
              return value;
         }
    }
    value = atoi((char *)string);
    return value;
}
bool GetStrArg(char *value, const char *name, const char *arg, int arglen)
{
    int namelen = strlen(name);

    if(namelen >= arglen)
	     return false;
    if(strncmp((char *)arg, (char *)name, namelen) == 0)
    {
	     memcpy(value, arg + namelen, arglen - namelen);
	     value[arglen - namelen] = '\0';
	     return true;
    }
    return false;
}
bool GetNumArg(unsigned long int *value, const char *name, const char *arg, int arglen)
{
    char buffer[256];

    if(GetStrArg(buffer, name, arg, arglen) == false)
	     return false;
    *value = ParseNumber(buffer);
    return true;
}
bool GetBoolArg(bool *value, const char *name, const char *arg, int arglen)
{
    char buffer[256];

    if(GetStrArg(buffer, name, arg, arglen) == false)
	     return false;
    if(strcmp(buffer, "false") == 0)
    {
	     *value = false;
	     return true;
    }
    else if(strcmp(buffer, "true") == 0)
    {
	     *value = true;
	     return true;
    }
    return false;
}
void ParseKernelParams(char *params)
{
    const char *arg = params;
    int argc = 0;
    int i;
    
    for(i = 0; ; ++i)
    {
	     int len;
	     if((params[i] != '\0') && (params[i] != ' '))
         {
	          continue;
         }
	     len = params + i - arg;
	     
         /* Add extra parametres here 
          *
          */
	     GetBoolArg(&SystemStatus.SerialDebug, "debug0=", arg, len);
	     GetNumArg(&SystemStatus.MemorySize, "ram=", arg, len);
	
	     if(params[i] == '\0')
         {
	          break;
         }
	     arg = params + i + 1;
	     argc++;
    }
}
