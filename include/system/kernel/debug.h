/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\debug.h
    @brief Debug functions
*/
#ifndef INCLUDE_SYSTEM_KERNEL_DEBUG_H
#define INCLUDE_SYSTEM_KERNEL_DEBUG_H

#include <kernel\console.h>
#include <kernel\x86\irq.h>
#include <drivers\char\serial.h>

/** @brief Error messages appear with this colour */
#define COLOUR_PANIC COLOUR_WHITE
/** @brief Serial port used to print debug messages */
#define SERIAL_DEBUG_PORT SERIAL_COM0_NUMBER
/** @brief Status bar background colour */
#define STBAR_BAR_COLOUR DEFAULT_COLOUR
/** @brief Status bar text colour */
#define STBAR_TEXT_COLOUR COLOUR_WHITE
/** @brief Status bar symbol */
#define STBAR_SYMBOL 179
/** @brief Status bar individual width */
#define STBAR_WIDTH 20

/** @fn void DebugInit()
    @brief Init kernel debug
    @ingroup sys_kernel_debug
*/
void DebugInit();
/** @fn extern void _Bug(const char *file, const char *function, unsigned int line)
    @brief Developing purposes only. Prints a message saying that we faced a BUG
    @ingroup sys_kernel_debug
    @param file File that called this
    @param function Function that called this
    @param line Function line
*/
extern void _Bug(const char *file, const char *function, unsigned int line);
/** @fn extern void _Panic(const char *file, const char *function, unsigned int line, x86RegistersStruct *regs, const char *format, ...)
    @brief Called when the systems faces a critical error. Halts the system and print some info
    @ingroup sys_kernel_debug
    @param file File that called this
    @param function Function that called this
    @param line Function line
    @param regs CPU registers status can be attached to the panic message
    @param format Unparsed string
*/
extern void _Panic(const char *file, const char *function, unsigned int line, x86RegistersStruct *regs, const char *format, ...);
/** @fn extern void PanicAssert(const char *file, const char *function, unsigned int line, const char *desc)
    @brief The system faced a logical error. Halts the system and print some info
    @ingroup sys_kernel_debug
    @param file File that called this
    @param function Function that called this
    @param line Function line
    @param desc Prints the expression that caused the error
*/
extern void PanicAssert(const char *file, const char *function, unsigned int line, const char *desc);
/** @fn extern void DebugPrintf(const char *file, const char *function, const char *fmt, ...)
    @brief Print a string using the serial debug port
    @ingroup sys_kernel_debug
    @param file File that called this
    @param function Function that called this
    @param fmt Unparsed string
*/
extern void DebugPrintf(const char *file, const char *function, const char *fmt, ...);
/** @fn void StatusBarInit()
    @brief Init status bar
    @ingroup sys_kernel_debug
*/
void StatusBarInit();
/** @fn void StBarMessage(unsigned int area, const char *format, ...)
    @brief Print a message in the designated status bar area
    @ingroup sys_kernel_debug
    @param area Status bar area The number of available areas is given by: VgaCurrentMode.width / (STBAR_WIDTH + 1)
    @param format String
*/
void StBarMessage(unsigned int area, const char *format, ...);

/** @brief Panic's the system: print an error message and halts the system
    @param Msg Error message. Multiple args */
#define PANIC(Msg...) _Panic(__FILE__, __FUNCTION__, __LINE__, (void *)0, Msg)
/** @brief Panic's the system: print an error message and the contents of CPU registers, halts the system
    @param Regs CPU Registers. See "include/system/kernel/i386/irq.h"
    @param Msg Error message. Multiple args */
#define RPANIC(Regs, Msg...) _Panic(__FILE__, __FUNCTION__, __LINE__, Regs, Msg)
/** @brief Verifies if B is true. Halts the system if not
    @param B Check if this condition is true */
#define KASSERT(B) ((B) ? (void)0 : PanicAssert(__FILE__, __FUNCTION__, __LINE__, #B))
/** @brief Prints a message through the standard debug, in default we use the serial COM1
    @param Msg Message to output. Multiple args */
#define DEBUG(Msg...) DebugPrintf(__FILE__, __FUNCTION__, Msg)
/** @brief The system has faced a Bug. Halts the system and prints a message */
#define BUG() _Bug(__FILE__, __FUNCTION__, __LINE__)

#endif
