/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\char\keyboard\keyboard.c
    @brief Keyboard driver
*/
#include <errno.h>
#include <ctype.h>
#include <kernel\console.h>
#include <kernel\debug.h>
#include <kernel\x86\io.h>
#include <kernel\x86\irq.h>
#include <drivers\char\keyboard.h>
#include "pt.h"

/** @fn static void KbWaitWrite(unsigned char data)
    @brief Wait until keyboard is ready then sends a data byte
    @ingroup sys_drivers_char_keyboard 
    @param data Data to sent
    @internal
*/
static void KbWaitWrite(unsigned char data)
{
    unsigned long timeout;
    unsigned char stat;

    for (timeout = 500000L; timeout != 0; timeout--) {
        stat = inportb(KEYB_CTRL);
        if ((stat & 0x02) == 0)
            break;
    }
    if (timeout != 0)
        outportb(KEYB_PORT, data);
}
/** @fn static void KeyboardHandlerIrq(x86RegistersStruct *r)
    @brief Keyboard IRQ handler. Called when keyboard IRQ is fired
    @ingroup sys_drivers_char_keyboard 
    @param r CPU registers values
    @internal
*/
static void KeyboardHandlerIrq(x86RegistersStruct *r)
{
    static int keypad[KEY_RAW_NUM0 - KEY_RAW_NUM7 + 1] =
    {
        7, 8, 9, -1,
        4, 5, 6, -1,
        1, 2, 3,
        0
    };
    static int alt;
    static int ctrl;
    static int shift;
    static unsigned long leds;
    static int extended;
    static int make;
    
    unsigned char scancode;
    unsigned short code; // 2 bytes
    unsigned int temp;
    int extended_temp;

    // Read keyboard data (1 byte) = (1 scancode)
	scancode = inportb(KEYB_PORT);
	code = scancode;
	temp = 0;
	
    switch(scancode) {
         //case 0x00: // Error or Buffer Overflow
         //case 0xAA: // Power On Self Test Passed (BAT Completed) 
         //case 0xEE: // See Echo Command (Host Commands) 
         //case 0xFA: // Acknowledge 
         case 0xFE: // Resend - Upon receipt of the resend command the Host should re-transmit the last byte sent
         //case 0xFF: // Error or Buffer Overflow
              return;//break; // ?
         case 0xE0:
              extended = 1; // Extended key coming
              return;
    }
    extended_temp = extended;
    if(extended)
         code |= 0xE000;
    extended = 0;

    if(scancode > 0x80) { // Released key
         code &= ~0x80; // Normal status
         switch(code) {
              case KEY_RAW_LCTRL:    // Control Released
              case KEY_RAW_RCTRL:
                   ctrl = 0;
                   break;
              case KEY_RAW_LALT:     // Alt Released
                   alt = 0;
                   break;
              case KEY_RAW_ALTGR:
                   alt = 0;
                   ctrl = 0;
                   break;
              case KEY_RAW_LSHIFT:   // Shift Released
              case KEY_RAW_RSHIFT:
                   shift = 0;
                   break;
         }
         return;
    }
    switch(code) {
         // E1 1D  45 E1  9D C5 - Pause 
         // E0 AA - break code sufix
         // E0 B7  E0 AA - Printscreen break
         // E0 2A - make code internal
         case KEY_RAW_MAKE:
              make = 1;
              break;//return;
         // E0 2A  E0 37 - Printscreen
         case KEY_RAW_PRTSCR:
              if(make) {
                   kprintf("[PRTSCR]");
                   make = 0;
              }
              break;//return;
         case KEY_RAW_LCTRL:    // Control Pressed
         case KEY_RAW_RCTRL:
              ctrl = 1;
              break;//return;
         case KEY_RAW_LALT:     // Alt Pressed
              alt = 1;
              break;//return;
         case KEY_RAW_ALTGR:
              alt = 1;
              ctrl = 1;
              break;//return;
         case KEY_RAW_LSHIFT:   // Shift Pressed
         case KEY_RAW_RSHIFT:
              shift = 1;
              break;//return;
         case KEY_RAW_LWIN:
              kprintf("[LWIN]");
              break;//return;
         case KEY_RAW_RWIN:
              kprintf("[RWIN]");
              break;//return;
         case KEY_RAW_MENU:
              kprintf("[MENU]");
              break;//return;
         case KEY_RAW_CAPS:     // Caps Lock Pressed
              leds ^= KBD_BUCKY_CAPS;
	          goto LEDS;
         case KEY_RAW_NUM:      // Num Lock Pressed
              leds ^= KBD_BUCKY_NUM;
              goto LEDS;
         case KEY_RAW_SCRL:     // Scroll Lock Pressed
              leds ^= KBD_BUCKY_SCRL;
LEDS:
              KbWaitWrite(KEYB_SET_LEDS);
              temp = 0;
              if (leds & KBD_BUCKY_SCRL)
                   temp |= 1;
              if (leds & KBD_BUCKY_NUM)
                   temp |= 2;
              if (leds & KBD_BUCKY_CAPS)
                   temp |= 4;
              KbWaitWrite(temp);
              break;//return;
    }
    
    temp = 0;
    
    switch(code) {
         case KEY_RAW_NUMSL:
              temp = '/';
              break;
         default:
              code &= ~0xE000; // No extended keys anymore
              if(code > 0x5D)
                  return; // Invalid code. It is out of KbPTmap range
              if((code >= KEY_RAW_NUM7) && 
                 (code <= KEY_RAW_NUM_DOT) &&
                 (code != 0x4A/*Num -*/) &&
                 (code != 0x4E/*Num +*/) &&
                 (!extended_temp)) {
                   if(leds & KBD_BUCKY_NUM) {
                        if(code == KEY_RAW_NUM_DOT)
                             temp = '.';
                        else
                             temp = '0' + keypad[code - KEY_RAW_NUM7];
                   } else
                        temp = KbPTmap[code].normal;
              }
              else if(shift == 1)
                   temp = KbPTmap[code].shift;
              else if((ctrl == 1) && (alt == 1))
                   temp = KbPTmap[code].altgr;
              else
                   temp = KbPTmap[code].normal;
              break;
    }
    if(leds & KBD_BUCKY_CAPS) {
         if (isupper(temp))
              temp = tolower(temp);
         else if (islower(temp))
              temp = toupper(temp);
    }
    if(temp)
         ConsoleInput(temp);
    make = 0; // When we reach here this values cant be used anymore
    extended = 0;
}
int KeyboardInstall()
{     
   	if(x86IrqInstallHandler(1, KeyboardHandlerIrq))
   	     return -EIO;
    
    // Clean Leds status
    unsigned int temp;
    KbWaitWrite(KEYB_SET_LEDS);
    temp = 0;
    KbWaitWrite(temp);
    
    // Limpa o buffer do teclado
	while(inportb(KEYB_CTRL) & 1)
		 inportb(KEYB_PORT);

	BUG();
    // Driver is not completely implemented:
    // The Pause/Break key is treated as Num Lock;
    // The PrtScreen does not work in some keyboards;
    // The special keys like Win or Menu are not added to the console buffer.
    return ESUCCESS;
}
int KeyboardUninstall()
{
    x86IrqUninstallHandler(1);
    return ESUCCESS;
}
