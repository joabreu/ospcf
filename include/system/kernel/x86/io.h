/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\io.h
    @brief Input/Output functions
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_IO_H
#define INCLUDE_SYSTEM_KERNEL_X86_IO_H

/** @fn unsigned char inportb(unsigned short _port)
    @brief Gets a byte from designated port
    @ingroup sys_kernel_arch_x86
    @param _port Port
    @return Byte
*/
unsigned char inportb(unsigned short _port);
/** @fn void outportb(unsigned short _port, unsigned char _data)
    @brief Outputs byte _data to port _port
    @ingroup sys_kernel_arch_x86
    @param _port Port
    @param _data Byte
*/
void outportb(unsigned short _port, unsigned char _data);

#endif
