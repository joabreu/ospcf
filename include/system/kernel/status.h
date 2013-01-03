/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\status.h
    @brief System status
*/
#ifndef INCLUDE_SYSTEM_KERNEL_STATUS_H
#define INCLUDE_SYSTEM_KERNEL_STATUS_H

#include <sys\types.h>
#include <kernel\rtc.h>
#include <kernel\x86\grub.h>

/** @brief Current System Status Structure */
typedef struct
{
    // Booting Stuff Info
    /** @brief Boot parameters */
    char KernelParams[4096];
    /** @brief Kernel start address */
    unsigned int KernelBegin;
    /** @brief Kernel end address (does not include modules, loaded on the run) */
    unsigned int KernelEnd;
    /** @brief System end address (= KernelEnd + Modules loaded on the fly) */
    unsigned int SystemEnd;
    /** @brief Kernel size in bytes */
    unsigned int KernelSize;
    /** @brief Modules count */
    unsigned int ModulesCount;
    /** @brief Multiboot passed info */
    MultibootInfoStruct *mbinfo;
    
    // Misc Stuff Info
    /** @brief Kernel Boot time. Defined during kernel boot */
    RtcTimeStruct KernelBootTime;
    // GDT, IDT
    // Multi Tasking Stuff
    /** @brief TRUE if in User Mode */
    bool UserMode;
    /** @brief TRUE if serial is in use for output data strings */ 
    bool SerialDebug;

    // Memory Stuff Info
    /** @brief Memory size in bytes */
    unsigned long int MemorySize;
    /** @brief Memory in use by paging in bytes */
    unsigned long int MemoryPaging;
    /** @brief Total pages for paging */
    unsigned int TotalPages;
    /** @brief Free pages */
    unsigned int FreePages;  
} SystemStatusStruct;
/** @brief System Information structure */
typedef struct
{
    /** @brief Kernel Version Number */
    unsigned int KernelVersion;
    /** @brief Kernel Build Number. Generated automatically. */
    unsigned int KernelBuild;
    /** @brief Kernel Version Name */
    char KernelVersionName[10];
    /** @brief Kernel File Name */ 
    char KernelName[15];
    /** @brief Kernel Build Date. Generated automatically. Format: MMM DD YYYY */
    char KernelBuildDate[15];
    /** @brief Kernel Build Time. Generated automatically. Format: HH:MM:SS */
    char KernelBuildTime[15];
    /** @brief Kernel Boot time. Defined during kernel boot */
    RtcTimeStruct KernelBootTime;
    // CpuInfoStruct
    /** @brief Total maped pages */
    unsigned int TotalPages;
    /** @brief Free Pages */
    unsigned int FreePages;
    /** @brief Kernel File size */
    unsigned int KernelSize;
    /** @brief TRUE if serial is in use for output data strings */
    bool SerialDebug;
} SystemInfoStruct;

/** @brief Current system status */
extern SystemStatusStruct SystemStatus;

/** @fn void InitSystemStatusStruct(MultibootInfoStruct *mbinfo)
    @brief Init system status struct
    @ingroup sys_kernel
    @param mbinfo System info provided by bootloader
*/
void InitSystemStatusStruct(MultibootInfoStruct *mbinfo);
/** @fn int GetSystemInfo(SystemInfoStruct *s)
    @brief Get system information
    @ingroup sys_kernel
    @param s Struct to store info
    @return Error code
*/
int GetSystemInfo(SystemInfoStruct *s);

#endif
