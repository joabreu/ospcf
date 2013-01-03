/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\version.h
    @brief Build information
*/
#ifndef INCLUDE_SYSTEM_KERNEL_VERSION_H
#define INCLUDE_SYSTEM_KERNEL_VERSION_H

#include <kernel\notes.txt>

/** @brief Kernel Build number (generated automaticaly) */ 
extern const unsigned int KernelBuild;
/** @brief Kernel Version Name */ 
extern const char *KernelVersionName;
/** @brief Kernel Current Version */ 
extern const unsigned int KernelVersion;
/** @brief Kernel Filename */ 
extern const char *KernelName;
/** @brief Kernel Build Date (generated automaticaly) */ 
extern const char *KernelBuildDate;
/** @brief Kernel Build Time (generated automaticaly) */ 
extern const char *KernelBuildTime;

#endif
