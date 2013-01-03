/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\shell\main.c
    @brief Internal shell. Version 2.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <os.h>
#include <kernel\debug.h>
#include <kernel\console.h>
#include <kernel\version.h>
#include <kernel\status.h>
#include <kernel\rtc.h>
#include <kernel\proc.h>
#include <drivers\video\vga.h>
#include <drivers\char\keyboard.h>
#include "tinysh.h"

/** @brief Shell title */
#define SHELL_TITLE	 "Minimal 'Internal' Shell - V 2.0\n[use 'help' for info]\n"

/** @fn static void ShellQuit(int argc, char **argv)
    @brief Termina a Linha de comandos
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellQuit(int argc, char **argv)
{
	exit(0);
    while(1)
         ;
}
/** @fn static void ShellReboot(int argc, char **argv)
    @brief Reinicializa o computador
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellReboot(int argc, char **argv)
{
    //x86SystemReboot();
}
/** @fn static void ShellAbout(int argc, char **argv)
    @brief Imprime informações de sistema
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellAbout(int argc, char **argv)
{
	kprintf("INFO DE SISTEMA :\n");
    kprintf("Versao: %s\n", KernelVersionName);
    kprintf("Build: %d\n", KernelBuild);
    kprintf("Data de compilacao: %s - %s\n", KernelBuildDate, KernelBuildTime);
    kprintf("Realeases Notes :\n%s\n", REALEASE_NOTES);
}
/** @fn static void ShellColor(int argc, char **argv)
    @brief Define a cor do PROMPT
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (Cor)
    @ingroup sys_kernel_shell 
*/
static void ShellColor(int argc, char **argv)
{
    if(argc < 2)
    {
         kprintf("Define a cor do PROMPT :\n");
         kprintf("--> %s [color]\n", argv[0]);
         return;
    }
    int color = atoi(argv[1]);
    /*tinysh.c*/PROMPT_COLOR = color;
}
/** @fn static void ShellError(int argc, char **argv)
    @brief Testa a função PANIC() gerando uma mensagem de erro
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (Mensagem)
    @ingroup sys_kernel_shell 
*/
static void ShellError(int argc, char **argv)
{
	if(argc < 2)
	{
         kprintf("Testa a funcao PANIC() gerando uma mensagem de erro :\n");
         kprintf("--> %s [description]\n", argv[0]);
         return;
    }
    PANIC(argv[1]);
}
/** @fn static void ShellVgaDemo(int argc, char **argv)
    @brief Demonstra funções teste do modo 320x250 VGA
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellVgaDemo(int argc, char **argv)
{
    VgaDemo();
}
/** @fn static void ShellKeyb(int argc, char **argv)
    @brief Imprime informações standard sobre o teclado
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellKeyb(int argc, char **argv)
{
    kprintf("Teclado instalado [%s]\n", GetKeyboardTypeString());
}
/** @fn static void ShellSysInfo(int argc, char **argv)
    @brief Imprime info. sobre o sistema
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellSysInfo(int argc, char **argv)
{
    SystemInfoStruct sys;
    GetSystemInfo(&sys);
    
    kprintf("Info. de sistema :\n");
    kprintf("KernelVersion : %d\t|   KernelBuild : %d\n", sys.KernelVersion, sys.KernelBuild);
    kprintf("KernelVersionName : %s\t|   KernelName : %s\n", sys.KernelVersionName, sys.KernelName);
    kprintf("BuildDate : %s\t|   BuildTime : %s\n", sys.KernelBuildDate, sys.KernelBuildTime);
    kprintf("KernelBootTime: %d-%d-%d %d:%d:%d [%d]\n", sys.KernelBootTime.DayMonth,
    sys.KernelBootTime.Month, sys.KernelBootTime.Year, sys.KernelBootTime.Hour, sys.KernelBootTime.Minute,
    sys.KernelBootTime.Second, sys.KernelBootTime.DayWeek);
    kprintf("TotalPages : %d\t|   FreePages : %d\n", sys.TotalPages, sys.FreePages);
    kprintf("KernelSize : %d Bytes\n", sys.KernelSize);
    kprintf("SerialDebug : %s\n", (sys.SerialDebug) ? "Enabled" : "Disabled");
}
/** @fn static void ShellSysStatus(int argc, char **argv)
    @brief Imprime info. ACTUALIZADA sobre o estado do sistema
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellSysStatus(int argc, char **argv)
{
    kprintf("Estado actual do sistema:\n");
    kprintf("KernelParams : %s\n", SystemStatus.KernelParams);
    kprintf("KernelBegin : 0x%x\t|   KernelEnd: 0x%x\n", SystemStatus.KernelBegin, SystemStatus.KernelEnd);
    kprintf("MemorySize : %d\t|   KernelSize: %d Bytes\n", SystemStatus.MemorySize, SystemStatus.KernelSize);
    kprintf("ModulesCount : %d\n", SystemStatus.mbinfo->ModulesCount);
    kprintf("TotalPages : %d\t|   FreePages : %d\n", SystemStatus.TotalPages, SystemStatus.FreePages);
    kprintf("SerialDebug : %s\t| UserMode : %s\n", ((SystemStatus.SerialDebug) ? "Enabled" : "Disabled"),
    ((SystemStatus.UserMode) ? "Yes" : "No"));
}
/** @fn static void ShellPF(int argc, char **argv)
    @brief Efectua uma Page Fault
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellPF(int argc, char **argv)
{
    kprintf("doing a page fault ...\n");
    unsigned int *zm = (unsigned int*)0xf010b000;
    int z = (int)*zm;
    kprintf("k = %d\n", z);
    return;
}
/** @fn static void ShellDate(int argc, char **argv)
    @brief Imprime a data actual
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (None)
    @ingroup sys_kernel_shell 
*/
static void ShellDate(int argc, char **argv)
{
    RtcTimeStruct t;
    if(SysGetDate(&t) == 0)
    {
         kprintf("Current Date: %d, %d(%d) of (%d), %d:%d:%d\n",
         t.Year, t.DayMonth, t.DayWeek, t.Month, t.Hour, t.Minute, t.Second);
    }
    t = SysGetBootTime();
    kprintf("Kernel Boot Time : %d, %d(%d) of (%d), %d:%d:%d\n",
    t.Year, t.DayMonth, t.DayWeek, t.Month, t.Hour, t.Minute, t.Second);
}
/** @fn static void ShellStBar(int argc, char **argv)
    @brief Imprime uma mensagem na barra de estado
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (area, message)
    @ingroup sys_kernel_shell 
*/
static void ShellStBar(int argc, char **argv)
{
    if(argc < 3)
    {
         kprintf("Usage: %s <area> <message>\n", argv[0]);
         return;
    }
    StBarMessage(atoi(argv[1]), argv[2]);
}
/** @fn static void ShellDebug(int argc, char **argv)
    @brief Imprime uma mensagem através do comando DEBUG()
    @param argc Número de argumentos passados
    @param argv Lista de argumentos passados (message)
    @ingroup sys_kernel_shell 
*/
static void ShellDebug(int argc, char **argv)
{
    if(argc < 2)
    {
         kprintf("Usage: %s <message>\n", argv[0]);
         return;
    }
    DEBUG(argv[1]);
}
/** @fn static void ShellProc(int argc, char **argv)
    @brief Print list of system processes
    @param argc Arguments count
    @param argv Arguments list (none)
    @ingroup sys_kernel_shell 
*/
static void ShellProc(int argc, char **argv)
{
    DumpProcesses();
}
/** @fn static void ShellPinfo(int argc, char **argv)
    @brief Print details about a process
    @param argc Arguments count
    @param argv Arguments list (pid)
    @ingroup sys_kernel_shell 
*/
static void ShellPinfo(int argc, char **argv)
{
    if(argc < 2)
    {
         kprintf("Usage: %s <pid>\n", argv[0]);
         return;
    }
    ProcessStruct *p;
    p = GetProcess(atoi(argv[1]));
    if(p != NULL)
         ProcessPrintInfo(p);
    else
         kprintf("Invalid PID or process not available\n");
}

/** @brief Shell Command structure: Quit Shell command 
    @internal
*/
static SHELLCMD quitcmd    = { 0, "quit",   "Termina a Linha de comandos", 0, ShellQuit, 0, 0, 0 };
/** @brief Shell Command structure: Reboot PC command 
    @internal
*/
static SHELLCMD sreboot    = { 0, "reboot", "Reinicializa o computador", 0, ShellReboot, 0, 0, 0 };
/** @brief Shell Command structure: About PCF command 
    @internal
*/
static SHELLCMD about      = { 0, "about",  "Imprime info do PCF", 0, ShellAbout, 0, 0, 0 };
/** @brief Shell Command structure: Set Prompt Colour command 
    @internal
*/
static SHELLCMD pcolor     = { 0, "pcolor", "Define a cor do PROMPT", "[color]", ShellColor, 0, 0, 0 };
/** @brief Shell Command structure: Panic System command 
    @internal
*/
static SHELLCMD paniccmd   = { 0, "panic",  "Testa a função PANIC() gerando uma mensagem de erro",
                           "[description]", ShellError, 0, 0, 0 };
/** @brief Shell Command structure: VGA controller test command 
    @internal
*/
static SHELLCMD vgacmd     = { 0, "vga",    "Demonstra funcoes teste do modo 320x250 VGA", 0, ShellVgaDemo,
                           0, 0, 0 };
/** @brief Shell Command structure: Keyboard Info command 
    @internal
*/
static SHELLCMD kbcmd      = { 0, "keyb",   "Imprime info standard sobre o teclado", 0, ShellKeyb, 0, 0, 0 };
/** @brief Shell Command structure: System info command 
    @internal
*/
static SHELLCMD infocmd    = { 0, "info", "Imprime info. sobre o sistema", 0, ShellSysInfo, 0, 0, 0 };
/** @brief Shell Command structure: System status command 
    @internal
*/
static SHELLCMD statuscmd  = { 0, "status", "Imprime info. ACTUALIZADA sobre o estado do sistema", 0,
                           ShellSysStatus, 0, 0, 0 };
/** @brief Shell Command structure: Generate Page Fault command 
    @internal
*/
static SHELLCMD pfcmd      = { 0, "pf", "Efectua uma Page Fault", 0, ShellPF, 0, 0, 0 };
/** @brief Shell Command structure: Print current date command 
    @internal
*/
static SHELLCMD datecmd    = { 0, "date", "Imprime a data actual", 0, ShellDate, 0, 0, 0 };
/** @brief Shell Command structure: Output a message in the status bar command 
    @internal
*/
static SHELLCMD stbarcmd   = { 0, "stbar", "Imprime uma mensagem na barra de estado", "[area] [message]",
                           ShellStBar, 0, 0, 0 };
/** @brief Shell Command structure: Debug command 
    @internal
*/
static SHELLCMD debugcmd   = { 0, "debug", "Imprime uma mensagem utilizando o comando DEBUG()", "[message]",
                           ShellDebug, 0, 0, 0 };
/** @brief Shell Command structure: Show processes command 
    @internal
*/
static SHELLCMD proccmd   = { 0, "proc", "Print system list of process", 0, ShellProc, 0, 0, 0 };
/** @brief Shell Command structure: Print detailed info about a process
    @internal
*/
static SHELLCMD pinfocmd   = { 0, "pinfo", "Print detailed info about a process", "[pid]", ShellPinfo, 0, 0, 0 };

/** @fn int ShellMain(int argc, char **argv)
    @brief Main function of process Shell 2.0
    @ingroup sys_kernel_shell 
    @param argc Arguments count
    @param argv Arguments list
    @return Zero
*/
int ShellMain(int argc, char **argv)
{
	tinysh_add_command(&quitcmd);
	tinysh_add_command(&sreboot);
    tinysh_add_command(&about);
	tinysh_add_command(&pcolor);
	tinysh_add_command(&paniccmd);
	tinysh_add_command(&vgacmd);
    tinysh_add_command(&kbcmd);
    tinysh_add_command(&infocmd);
    tinysh_add_command(&statuscmd);
    tinysh_add_command(&pfcmd);
    tinysh_add_command(&datecmd);
    tinysh_add_command(&stbarcmd);
    tinysh_add_command(&debugcmd);
    tinysh_add_command(&proccmd);
    tinysh_add_command(&pinfocmd);

	kprintf("%s", SHELL_TITLE);

	int tmp = 0;
    tinysh_char_in('\r');	
	while(1)
	{
        tmp = getchar();
        if(tmp == 0xE0) // Extended key coming
              tinysh_char_in((getchar() | KB_SPECIAL));
		else
              tinysh_char_in(tmp);
    }
	return 0;
}
