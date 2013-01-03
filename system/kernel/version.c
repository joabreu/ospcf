/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\version.c
    @brief Kernel version information
*/
#include <kernel\version.h>

const unsigned int KernelVersion = 4; // Version 4: Beggined at 07/2010
const char *KernelVersionName = "DEVO";
const char *KernelName = "kernel.out";
const char *KernelBuildDate = __DATE__;
const char *KernelBuildTime = __TIME__;
