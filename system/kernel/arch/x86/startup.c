/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\arch\x86\startup.c
    @brief Kernel startup functions
*/
#include <string.h>
#include <kernel\kmain.h>
#include <kernel\debug.h>
#include <kernel\console.h>
#include <kernel\syscall.h>
#include <kernel\status.h>
#include <kernel\timer.h>
#include <kernel\kparams.h>
#include <kernel\rtc.h>
#include <kernel\proc.h>
#include <kernel\x86\grub.h>
#include <kernel\x86\gdt.h>
#include <kernel\x86\idt.h>
#include <kernel\x86\isr.h>
#include <kernel\x86\cpu.h>
#include <kernel\x86\paging.h>
#include <drivers\video\vga.h>
#include <drivers\char\serial.h>
#include <drivers\char\keyboard.h>

extern unsigned int PlacementAddress;

/** @fn void x86KernelStartup(MultibootInfoStruct *mbinfo, unsigned int magic, unsigned int esp)
    @brief Kernel entry point. Performs arch dependent operations.
    @ingroup sys_kernel_arch_x86
    @param mbinfo Multiboot info provided by GRUB
    @param magic Magic number, for sanity check
    @param esp Kernel initial esp
*/
void x86KernelStartup(MultibootInfoStruct *mbinfo, unsigned int magic, unsigned int esp)
{
/******************************************************************************/
/* Critical section, no interrupts, no kheap, no data debug functions */
    x86GdtInstall();
    x86IdtInstall();
    x86IsrInstall();
    x86IrqInstall();
    if(mbinfo->ModulesCount > 0) {
         unsigned int new_base = 0;
         new_base = ((struct MultibootModuleStruct *)(mbinfo->ModulesAddr))[mbinfo->ModulesCount - 1].ModuleEnd;
/******************************************************************************/
/* Until this operation occurs is not recommended to use the kheap functions */
         PlacementAddress = new_base;
    }
/* END critical section */
/******************************************************************************/
    VgaInit();
    ConsoleInit();
    kprintfc(COLOUR_WHITE, "Operating System PCF - Arch: i386 <-> Booting Kernel\n");
    kprintfc(COLOUR_WHITE, "Copyright (C) 2008-2010 Miguel Abreu. Licensed under GPL v3.\n\n");
    
    if(magic != 0x2BADB002) // Illegal magic number
         PANIC("ILLEGAL MAGIC NUMBER!");
         
    kprintf("booting from: %s\n", (char *)mbinfo->BootLoaderName);
/******************************************************************************/
/* Parse kernel parameters, init data debug */
    memset(&SystemStatus, 0, sizeof(SystemStatusStruct));
    if(mbinfo->CmdLine != 0) {
         strncpy(SystemStatus.KernelParams, (char *)mbinfo->CmdLine, 4096);
         kprintf("kernel params:   '%s'\n", SystemStatus.KernelParams);
         ParseKernelParams(SystemStatus.KernelParams);
    }
    InitSystemStatusStruct(mbinfo);
    INIT(SerialInit());
    DebugInit();
    StatusBarInit();
/******************************************************************************/
    kprintfc(COLOUR_LIGHTCYAN, "*** Loading some important stuff\n");
    x86PagingInit(SystemStatus.MemorySize);kprintf("paging: using %d bytes\n", SystemStatus.MemoryPaging);
    StBarMessage(3, "%d MB", SystemStatus.MemorySize / ( 1024 *1024 ));
    SyscallInstall();kprintf("syscalls: at 0x80\n");
    SetProcessingStatus(1);
    INIT(ProcessingInit());
    // Device manager
    INIT(TimerInstall(1000));kprintf("timer: at 1000 Hz\n");
    INIT(RtcInit(1));
    INIT(KeyboardInstall());kprintf("keyboard: detected %s\n", GetKeyboardTypeString());
/******************************************************************************/
    kprintfc(COLOUR_LIGHTGREEN, "*** ");
    kprintf("Enabling interrupts ...\n");STI();
/******************************************************************************/
/* From now on: interrupts enabled. The arch dependent stuff is done,
   pass control to main kernel */
    KernelMain();
    PANIC("KernelMain() returned");
    while(1)
         ;
} 
