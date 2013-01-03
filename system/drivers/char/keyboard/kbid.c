/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\char\keyboard\kbid.c
    @brief Keyboard driver
*/
#include <kernel\x86\io.h>
#include <drivers\char\keyboard.h>

/** @fn static int KbWait()
    @brief Keyboard delay
    @ingroup sys_drivers_char_keyboard 
    @return Keyboard buffer status
    @internal
*/
static int KbWait()
{
    unsigned i;
    int tmp;
    for(i=0; i < 0xffff; i++) { // Reasonable waiting loop
         tmp = inportb(KEYB_CTRL);       // Get status port
         if((tmp & 0x01) == 0x01)	// If char waiting in buffer
              return (tmp & 0x01);	    // Go back
    }
    return(tmp & 0x01);		    // No char after waiting: go back
}
int GetKeyboardType()
{
    int ret_code;

    outportb(0x21, 0x02); // Block IRQ1
    outportb(KEYB_PORT, 0xf2); // Give command
    if(!KbWait())
         ret_code = 0;    // XT-keyboard
    else {
         if(inportb(KEYB_PORT) != 0xfa)
              ret_code = 4;	  // No ACK: error
         else {
              if(!KbWait())
	               ret_code = 1;  // No ID byte: normal AT keyboard
              else {
	               if(inportb(KEYB_PORT) != 0xab)
	                    ret_code = 5; 	  // Unknown ID byte: error
                   else {
	                    if(!KbWait())
	                         ret_code = 6; // No second ID byte: error
                        else {
	                         if(inportb(KEYB_PORT) != 0x41)
	                              ret_code = 7;	 // Unknown second ID byte: error
                             else
	                              ret_code = 2;	 // MF II keyboard
                        }
                   }
              }
         }
    }
    outportb(0x21,0x00);  // Reenable IRQ1
    return (ret_code);
}
char *GetKeyboardTypeString()
{
    int result;
    result = GetKeyboardType();
    switch(result) {
         case 0: return "XT Keyboard";
         case 1: return "AT Keyboard";
         case 2: return "MF II Keyboard";
         default: return "Unknown Keyboard";
    }
}
