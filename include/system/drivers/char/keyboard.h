/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\drivers\char\keyboard.h
    @brief Keyboard driver
*/
#ifndef INCLUDE_SYSTEM_DRIVERS_CHAR_KEYBOARD_H
#define INCLUDE_SYSTEM_DRIVERS_CHAR_KEYBOARD_H

/** @brief General Keyboard Port */ 
#define KEYB_PORT               0x60
/** @brief Keyboard Controler port */ 
#define KEYB_CTRL               0x64

/** @brief Command: Set keyboard leds */ 
#define KEYB_SET_LEDS			0xED

// Special keys are not printed but you can do an *if* to check if they were pressed
/** @brief Flag: Special Key */ 
#define KB_SPECIAL            0x8000 // 32 768
// Available (65 535 - 32 768)
/** @brief Key: Esc */ 
#define KEY_ESC               1
/** @brief Key: F1 */ 
#define KEY_F1                59
/** @brief Key: F2 */ 
#define KEY_F2                60
/** @brief Key: F3 */ 
#define KEY_F3                61
/** @brief Key: F4 */ 
#define KEY_F4                62
/** @brief Key: F5 */ 
#define KEY_F5                63
/** @brief Key: F6 */ 
#define KEY_F6                64
/** @brief Key: F7 */ 
#define KEY_F7                65
/** @brief Key: F8 */ 
#define KEY_F8                66
/** @brief Key: F9 */ 
#define KEY_F9                67
/** @brief Key: F10 */ 
#define KEY_F10               68
/** @brief Key: F11 */ 
#define KEY_F11               87
/** @brief Key: F12 */ 
#define KEY_F12               88
/** @brief Key: Insert */ 
#define KEY_INSERT            82
/** @brief Key: Delete */ 
#define KEY_DELETE            83
/** @brief Key: Home */ 
#define KEY_HOME              71
/** @brief Key: End */ 
#define KEY_END               79
/** @brief Key: Page Up */ 
#define KEY_PAGEUP            73
/** @brief Key: Page Down */ 
#define KEY_PAGEDOWN          81
/** @brief Key: Left Arrow */ 
#define KEY_LEFTARROW         75
/** @brief Key: Up Arrow */ 
#define KEY_UPARROW           72
/** @brief Key: Down Arrow */ 
#define KEY_DOWNARROW         80
/** @brief Key: Right Arrow */ 
#define KEY_RIGHTARROW        77
/** @brief Key: Print Screen */ 
#define KEY_PRINTS            55
/** @brief Key: Left Windows */ 
#define KEY_LWIN              90 // !!!!!!!!
/** @brief Key: Right Windows */ 
#define KEY_RWIN              91 // !!!!!!!!
/** @brief Key: Menu */ 
#define KEY_MENU              92 // !!!!!!!!

/** @brief Keyboard type: XT keyboard */ 
#define KEYB_TYPE_XT          0
/** @brief Keyboard type: AT keyboard */ 
#define KEYB_TYPE_AT          1
/** @brief Keyboard type: MF II keyboard */ 
#define KEYB_TYPE_MF_II       2

/** @fn int KeyboardInstall()
    @brief Install and inits keyboard driver. Set leds and install irq
    @ingroup sys_drivers_char_keyboard
    @return Error code
*/
int KeyboardInstall();
/** @fn int KeyboardUninstall()
    @brief Uninstall keyboard irq handler
    @ingroup sys_drivers_char_keyboard
    @return Error code
*/
int KeyboardUninstall();
/** @fn int GetKeyboardType()
    @brief Return keyboard type id
    @ingroup sys_drivers_char_keyboard
    @return Keyboard type
*/
int GetKeyboardType();
/** @fn char *GetKeyboardTypeString()
    @brief Return string with keyboard type
    @ingroup sys_drivers_char_keyboard
    @return Keyboard type string
*/
char *GetKeyboardTypeString();

#endif
