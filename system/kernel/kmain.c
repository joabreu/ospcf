/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\kmain.c
    @brief Kernel main functions
*/
#include <stdlib.h>
#include <string.h>
#include <kernel\debug.h>
#include <kernel\version.h>
#include <kernel\console.h>
#include <kernel\rtc.h>
#include <kernel\status.h>
#include <kernel\proc.h>
#include <kernel\mem\uheap.h>

int ShellMain(int argc, char **argv);

int thrdtest(int argc, char **argv)
{
    unsigned long count = 0;
    
    kprintf("ThrdTest: argc=%d\n", argc);
    if(argc > 0) {
         int pos = atoi(argv[0]);
         while(1) {
              StBarMessage(pos, "%x", count);
              count++;
         }
    }
    while(1);
}
/** @fn int KernelInit1(int argc, char **argv)
    @brief Kernel intialization process number 1
    @ingroup sys_kernel
    @param argc Arguments count
    @param argv Arguments list
    @return Exit error
*/
int KernelInit1(int argc, char **argv)
{
    
    kprintf("\nKernel Init Process v2.0\n");
    kprintf("Copyright (C) 2010 Miguel Abreu. Licensed under GPL v3.\n\n");
    ProcessRename("Init v2.0");
    
    if(SystemStatus.mbinfo->ModulesCount > 0) {
         unsigned int app_test_begin = *((unsigned int *)SystemStatus.mbinfo->ModulesAddr);
         unsigned int app_test_end = *((unsigned int *)(SystemStatus.mbinfo->ModulesAddr+4));
         unsigned int app_size = app_test_end - app_test_begin;
         unsigned int addr = umalloc(app_size);
         kprintf("app_begin=0x%x, app_end=0x%x, app_size=0x%x\n", app_test_begin, app_test_end, app_size);
         kprintf("new addr=0x%x\n", addr);
         memcpy(addr, app_test_begin, app_size);
         ProcessCreate("User", NULL, NORMAL_PRIO, addr, PRIV_USER);
    
         char *args0[] = { "3", 0};
         unsigned long pid = ProcessCreate("Test", (unsigned long)args0, NORMAL_PRIO, thrdtest, PRIV_KERNEL);
    } else {
         kprintf("no modules to load\n");
    }
    /*char *args[] = { "1", 0};
    ThreadCreate(GetCurrentProcess(), thrdtest, (unsigned long)args);
    char *args2[] = { "2", 0};
    ThreadCreate(GetCurrentProcess(), thrdtest, (unsigned long)args2);
    *///char *args3[] = { "3", 0};
    //ThreadCreate(GetCurrentProcess(), thrdtest, (unsigned long)args3);
    ProcessCreate("Shell 2.0", NULL, NORMAL_PRIO, ShellMain, PRIV_KERNEL);

    int i = 21474;
    while(i--)
         StBarMessage(0, "%x", i);
    
    //ProcessSleep(10*1000);
    //kprintf("Now killing process 'Test': ret=%d\n", ProcessKill(pid));
    return 0;
}
void KernelMain()
{
    kprintfc(COLOUR_WHITE, "\n\
Operating System PCF - Booting Kernel\n\
Copyright (C) 2008-2010 Miguel Abreu. Licensed under GPL v3.\n\
version:");
    kprintfc(COLOUR_LIGHTGREEN, "%d", KernelVersion);
    kprintfc(COLOUR_WHITE, " / name:");
    kprintfc(COLOUR_LIGHTGREEN, "%s", KernelVersionName);
    kprintfc(COLOUR_WHITE, " / build:");
    kprintfc(COLOUR_LIGHTGREEN, "%d\n", KernelBuild);
    kprintfc(COLOUR_WHITE, "build date: %s - %s\n\n", KernelBuildDate, KernelBuildTime);
    
    // Install vfs
    // Install fat
    // Install rootfs
    // Install devfs
    // Important paths
    // Install modules
    // Install devices that use the fs
    // Install tty as a device ?
    SysGetDate(&SystemStatus.KernelBootTime);
    RtcTimeStruct t = SysGetBootTime();
    kprintf("boot time: %d-%d-%d %d:%d:%d [%d]\n\n", t.DayMonth, t.Month, t.Year, t.Hour, t.Minute, t.Second, t.DayWeek);
    DEBUG("boot time: %d-%d-%d %d:%d:%d [%d]", t.DayMonth, t.Month, t.Year, t.Hour, t.Minute, t.Second, t.DayWeek);

    kprintfc(COLOUR_LIGHTCYAN, "*** Creating Important Processes");    
    ProcessCreate("Start process", NULL, NORMAL_PRIO, KernelInit1, PRIV_KERNEL);
    kprintfc(COLOUR_LIGHTCYAN, "\r\t\t\t\t\t\t\t\t[done]\n");
    kprintfc(COLOUR_WHITE, "-- Kernel Loaded --\n\n");
    SetProcessingStatus(0);
    while(1)
         ;
}
