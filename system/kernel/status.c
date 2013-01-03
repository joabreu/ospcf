/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\status.c
    @brief System status
*/
#include <errno.h>
#include <string.h>
#include <kernel\debug.h>
#include <kernel\status.h>
#include <kernel\version.h>
#include <kernel\console.h>

SystemStatusStruct SystemStatus;
extern unsigned int PlacementAddress;
/** @brief Kernel physical end. Defined during compilation. */
extern unsigned int end;
/** @brief Kernel physical begin. Defined during compilation */
extern unsigned int code;

void InitSystemStatusStruct(MultibootInfoStruct *mbinfo)
{
    unsigned long int ram_size = 0;
        
    SystemStatus.mbinfo = mbinfo;
    SystemStatus.ModulesCount = mbinfo->ModulesCount;
    SystemStatus.KernelBegin = (unsigned int)&code;
    SystemStatus.KernelEnd = (unsigned int)&end; // Physical end
    SystemStatus.SystemEnd = (unsigned int)PlacementAddress; // system end
    SystemStatus.KernelSize = SystemStatus.KernelEnd - SystemStatus.KernelBegin;
    ram_size = ((mbinfo->MemoryLower + mbinfo->MemoryUpper)*1024); // Bytes of ram
    if(SystemStatus.MemorySize == 0)
         SystemStatus.MemorySize = ram_size;
    else
         if(SystemStatus.MemorySize > ram_size)
         {
              // Kernel params
              SystemStatus.MemorySize = ram_size;
         }
    kprintfc(COLOUR_LIGHTGREEN, "[ Kernel is %d KB long ( 0x%x -> 0x%x ) ]\n",
    (SystemStatus.KernelSize / 1024), SystemStatus.KernelBegin, SystemStatus.KernelEnd);
    kprintf("system end at: 0x%x\n", SystemStatus.SystemEnd);
    kprintfc(COLOUR_LIGHTCYAN, "*** System has %d bytes of memory\n    Using %d bytes\n", ram_size, SystemStatus.MemorySize);    
}
int GetSystemInfo(SystemInfoStruct *s)
{
    if(!s)
         return -EINVAL;
         
    memset(s, 0, sizeof(SystemInfoStruct));
    
    s->KernelVersion = KernelVersion;
    s->KernelBuild = KernelBuild;
    strncpy(s->KernelVersionName, (char *)KernelVersionName, 10);
    strncpy(s->KernelName, (char *)KernelName, 15);
    strncpy(s->KernelBuildDate, (char *)KernelBuildDate, 15);
    strncpy(s->KernelBuildTime, (char *)KernelBuildTime, 15);
    s->KernelBootTime = SystemStatus.KernelBootTime;
    s->TotalPages = SystemStatus.TotalPages;
    s->FreePages = SystemStatus.FreePages;
    s->KernelSize = SystemStatus.KernelSize;
    s->SerialDebug = SystemStatus.SerialDebug;
    
    return ESUCCESS;
}
