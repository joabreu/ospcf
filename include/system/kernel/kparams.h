/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\kparams.h
    @brief Kernel params parser
*/
#ifndef INCLUDE_SYSTEM_KERNEL_KPARAMS_H
#define INCLUDE_SYSTEM_KERNEL_KPARAMS_H

#include <sys\types.h>

/** @fn bool GetStrArg(char *value, const char *name, const char *arg, int arglen)
    @brief Checks if argument name is present in string arg. Stores its value (a string) in value
    @ingroup sys_kernel 
    @param value Argument value
    @param name Argument name to find
    @param arg String
    @param arglen String len
    @return true if argument is found, false if not
*/
bool GetStrArg(char *value, const char *name, const char *arg, int arglen);
/** @fn bool GetNumArg(unsigned long int *value, const char *name, const char *arg, int arglen)
    @brief Checks if argument name is present in string arg. Stores its value (a number) in value.
    
    This function supports sufixes like 'k' (value*1024) or 'M' (value*1024*1024)
    @ingroup sys_kernel 
    @param value Argument value
    @param name Argument name to find
    @param arg String
    @param arglen String len
    @return true if argument is found, false if not
*/
bool GetNumArg(unsigned long int *value, const char *name, const char* arg, int arglen);
/** @fn bool GetBoolArg(bool *value, const char *name, const char *arg, int arglen)
    @brief Checks if argument name is present in string arg. Stores its value (true or false) in value.
    @ingroup sys_kernel 
    @param value Argument value
    @param name Argument name to find
    @param arg String
    @param arglen String len
    @return true if argument is found, false if not
*/
bool GetBoolArg(bool *value, const char *name, const char *arg, int arglen);
/** @fn void ParseKernelParams(char *params)
    @brief Parse the kernel params
    @ingroup sys_kernel 
    @param params Params string
*/
void ParseKernelParams(char *params);


#endif
