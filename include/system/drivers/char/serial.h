/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\drivers\char\serial.h
    @brief Serial port interface
*/
#ifndef INCLUDE_SYSTEM_DRIVERS_CHAR_SERIAL_H
#define INCLUDE_SYSTEM_DRIVERS_CHAR_SERIAL_H

/** @brief Serial COM1 internal number */ 
#define SERIAL_COM0_NUMBER 0
/** @brief Serial COM2 internal number */ 
#define SERIAL_COM1_NUMBER 1

// Serial Types
/** @brief Serial Type: Not present */ 
#define SERIAL_NONE     0
/** @brief Serial Type: 8250 */
#define SERIAL_8250     1
/** @brief Serial Type: 16450 */ 
#define SERIAL_16450	2
/** @brief Serial Type: 16550 */ 
#define SERIAL_16550	3
/** @brief Serial Type: 16550A */
#define SERIAL_16550A	4

/** @brief Serial COM1 Base address */ 
#define SERIAL_COM0_BASE    0x3f8
/** @brief Serial COM2 Base address */
#define SERIAL_COM1_BASE    0x2f8

/** @brief Serial LCR */
#define SERIAL_LCR      3
/** @brief Serial SCR */
#define SERIAL_SCR      7
/** @brief Serial FCR */
#define SERIAL_FCR      2

/** @fn char SerialRead(int number)
    @brief Reads a byte from serial port. Loop if there is no data
    @ingroup sys_drivers_char_serial
    @param number Serial port number
    @return Byte read
*/
char SerialRead(int number);
/** @fn void SerialWrite(int number, char a)
    @brief Send a byte to serial port, loop until device is not ready
    @ingroup sys_drivers_char_serial
    @param number Serial port number
    @param a Byte
*/
void SerialWrite(int number, char a);
/** @fn unsigned long int SerialDetect(int number)
    @brief Returns serial type of designated port
    @ingroup sys_drivers_char_serial
    @param number Serial port number
    @return Serial type
*/
unsigned long int SerialDetect(int number);
/** @fn char* SerialName(unsigned long int type)
    @brief Reads serial type and return it in a string
    @ingroup sys_drivers_char_serial
    @param type Serial type
    @return String
*/
char* SerialName(unsigned long int type);
/** @fn int SerialInit()
    @brief Inits serial driver
    @ingroup sys_drivers_char_serial
    @return Error code
*/
int SerialInit();

#endif
