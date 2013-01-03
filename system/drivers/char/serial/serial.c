/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\char\serial\serial.c
    @brief Serial port interface
*/
#include <errno.h>
#include <string.h>
#include <kernel\console.h>
#include <kernel\x86\io.h>
#include <drivers\char\serial.h>

/** @brief Serial Port struct */
struct SerialPortStruct
{
    /** @brief TRUE if this serial port was inited */
    bool inited;
    /** @brief Port base address */
    unsigned long int PortBase;
    /** @brief Type of UART controller */
    unsigned long int TypeUart;
    /** @brief Time to wait before send/receive data again */
    unsigned long int BaudRate;
};
/** @brief COM ports structure. We can control 2 COM ports */
struct SerialPortStruct ComPort[2];

/** @fn static int SerialReceived(unsigned int base)
    @brief Get's the last byte received by selected serial port, 0 if buffer is empty
    @ingroup sys_drivers_char_serial 
    @param base Serial base
    @return Byte
    @internal
*/
static int SerialReceived(unsigned int base)
{
    return inportb(base + 5) & 1;
}
char SerialRead(int number)
{
    if((number >= 0) && (number < 2))
         if(ComPort[number].inited == true) {
              while(SerialReceived(ComPort[number].PortBase) == 0)
                   ;
              return inportb(ComPort[number].PortBase);
         }
    return 0;
}
/** @fn static int SerialIsTransmitEmpty(unsigned int base)
    @brief True if selected serial port buffer is empty
    @ingroup sys_drivers_char_serial 
    @param base Serial base
    @return TRUE or FALSE
    @internal
*/
static int SerialIsTransmitEmpty(unsigned int base)
{
    return inportb(base + 5) & 0x20;
}
void SerialWrite(int number, char a)
{
    if((number >= 0) && (number < 2))
         if(ComPort[number].inited == true) {
              while(SerialIsTransmitEmpty(ComPort[number].PortBase) == false)
                   ;
              outportb(ComPort[number].PortBase,a);
         }
}
unsigned long int SerialDetect(int number)
{
    switch(number) {
         case SERIAL_COM0_NUMBER: return ComPort[SERIAL_COM0_NUMBER].TypeUart;
         case SERIAL_COM1_NUMBER: return ComPort[SERIAL_COM1_NUMBER].TypeUart;
    }
    return SERIAL_NONE;
}          
/** @fn static unsigned long int SerialDetectUnsafe(unsigned long int port)
    @brief Returns serial type of designated port base
    @ingroup sys_drivers_char_serial 
    @param port Serial port base
    @return Serial type
    @internal
*/
static unsigned long int SerialDetectUnsafe(unsigned long int port)
{
	unsigned char save = 0, data = 0;
	unsigned long int scr = 1;

	save = inportb(port + SERIAL_LCR);
	outportb(port + SERIAL_LCR, 0x1B);
	data = inportb(port + SERIAL_LCR);
	if(data != 0x1B)
		 return SERIAL_NONE;
	outportb(port + SERIAL_LCR, 0x03);
	data = inportb(port + SERIAL_LCR);
	if(data != 0x03)
		 return SERIAL_NONE;
	outportb(port + SERIAL_LCR, save);
	
    save = inportb(port + SERIAL_SCR);
	// check whether there is a scratch register.
	outportb(port + SERIAL_LCR, 0x55);
	data = inportb(port + SERIAL_LCR);
	if(data != 0x55)
		 scr = 0;
	outportb(port + SERIAL_LCR, 0xAA);
	data = inportb(port + SERIAL_LCR);
	if(data != 0xAA)
		 scr = 0;
	outportb(port + SERIAL_SCR, save);

	// Tenta activar os FIFOS para saber se são suportados
	outportb(port + SERIAL_FCR, 1);
	// Lê o estado do FIFO para saber se são suportados
	data = inportb(port + SERIAL_FCR);
	// Desabilita FIFOS para o caso de ser suportado
	outportb(port + SERIAL_FCR, 0);
		 	 
	// For the 16550A, bit 6 (and 7) of IIR is set when FIFOs are enabled
	if(data & 0x40)
		 return SERIAL_16550A;
	// For the 16550, bit 7 of IIR is set when FIFOs are enabled
	if(data & 0x80)
		 return SERIAL_16550;
	// We normally distinguish 8250 and 16450 by the scratch register,
	// as the 16450 introduced the scratch register.
	// However, some 8250s may also have it (and some 16450s may not),
	// so this test may not always be accurate.
	if(scr)
		 return SERIAL_16450;
	return SERIAL_8250;
}
char* SerialName(unsigned long int type)
{
	switch(type) {
		 case SERIAL_NONE:
			  return "Null";
		 case SERIAL_8250:
			  return "8250";
		 case SERIAL_16450:
			  return "16450";
		 case SERIAL_16550:
			  return "16550";
		 case SERIAL_16550A:
			  return "16550A";
    }
    return "Unknown";
}
int SerialInit()
{
	unsigned long int type = 0;
	int detected = 0;

	memset(ComPort, 0, (sizeof(struct SerialPortStruct)*2));
    type = SerialDetectUnsafe(SERIAL_COM0_BASE);
	if(type != SERIAL_NONE) {
		 ComPort[SERIAL_COM0_NUMBER].inited = true;
		 ComPort[SERIAL_COM0_NUMBER].PortBase = SERIAL_COM0_BASE;
		 ComPort[SERIAL_COM0_NUMBER].TypeUart = type;
		 ComPort[SERIAL_COM0_NUMBER].BaudRate = 38400;

         // Init the port
         outportb(SERIAL_COM0_BASE + 1, 0x00);    // Disable all interrupts
         outportb(SERIAL_COM0_BASE + 3, 0x80);    // Enable DLAB (set baud rate divisor)
         outportb(SERIAL_COM0_BASE + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
         outportb(SERIAL_COM0_BASE + 1, 0x00);    //                  (hi byte)
         outportb(SERIAL_COM0_BASE + 3, 0x03);    // 8 bits, no parity, one stop bit
         outportb(SERIAL_COM0_BASE + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
         outportb(SERIAL_COM0_BASE + 4, 0x0B);    // IRQs enabled, RTS/DSR set

         detected++;
         kprintf("serial: detected COM%d UART%s\n", SERIAL_COM0_NUMBER, SerialName(type));
	}
	type = SerialDetectUnsafe(SERIAL_COM1_BASE);
	if(type != SERIAL_NONE) {
		 ComPort[SERIAL_COM1_NUMBER].inited = true;
		 ComPort[SERIAL_COM1_NUMBER].PortBase = SERIAL_COM1_BASE;
		 ComPort[SERIAL_COM1_NUMBER].TypeUart = type;
		 ComPort[SERIAL_COM1_NUMBER].BaudRate = 38400;
		 
         // Init the port
         outportb(SERIAL_COM1_BASE + 1, 0x00);    // Disable all interrupts
         outportb(SERIAL_COM1_BASE + 3, 0x80);    // Enable DLAB (set baud rate divisor)
         outportb(SERIAL_COM1_BASE + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
         outportb(SERIAL_COM1_BASE + 1, 0x00);    //                  (hi byte)
         outportb(SERIAL_COM1_BASE + 3, 0x03);    // 8 bits, no parity, one stop bit
         outportb(SERIAL_COM1_BASE + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
         outportb(SERIAL_COM1_BASE + 4, 0x0B);    // IRQs enabled, RTS/DSR set
		 
		 detected++;
		 kprintf("serial: detected COM%d UART%s\n", SERIAL_COM1_NUMBER, SerialName(type));
	}
	if (detected == 0) {
		 kprintf("serial: no serial ports detected\n");
		 return ESUCCESS;
	}
	return ESUCCESS;
}
