/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\drivers\char\keyboard\pt.h
    @brief Portuguese keyboard layout
*/
/* These are checked auto in the IrqHandler so they need to have the value of the scancode */
/** @brief Key Code: Left Control */
#define	KEY_RAW_LCTRL	0x1D
/** @brief Key Code:  Left Shift */
#define	KEY_RAW_LSHIFT	0x2A
/** @brief Key Code:  Right Shift */
#define	KEY_RAW_RSHIFT	0x36
/** @brief Key Code: Left Alt */
#define	KEY_RAW_LALT	0x38
/** @brief Key Code:  Caps Lock */
#define	KEY_RAW_CAPS	0x3A
/** @brief Key Code:  Num Lock */
#define KEY_RAW_NUM     0x45
/** @brief Key Code:  Scroll Lock*/
#define KEY_RAW_SCRL    0x46
/** @brief Key Code:  Num Lock 7 */
#define KEY_RAW_NUM7	0x47
/** @brief Key Code:  Num Lock 8 */
#define KEY_RAW_NUM8	0x48
/** @brief Key Code:  Num Lock 9 */
#define KEY_RAW_NUM9	0x49
/** @brief Key Code:  Num Lock 4 */
#define KEY_RAW_NUM4	0x4b
/** @brief Key Code:  Num Lock 5 */
#define KEY_RAW_NUM5	0x4c
/** @brief Key Code:  Num Lock 6 */
#define KEY_RAW_NUM6	0x4d
/** @brief Key Code:  Num Lock 1 */
#define KEY_RAW_NUM1	0x4f
/** @brief Key Code:  Num Lock 2 */
#define KEY_RAW_NUM2	0x50
/** @brief Key Code:  Num Lock 3 */
#define KEY_RAW_NUM3	0x51
/** @brief Key Code:  Num Lock 0 */
#define KEY_RAW_NUM0	0x52
/** @brief Key Code:  Num Lock Dot */
#define KEY_RAW_NUM_DOT 0x53
/** @brief Key Code: Right Control */
#define	KEY_RAW_RCTRL	0xE01D
/** @brief Key Code: Make code internal prefix */
#define KEY_RAW_MAKE    0xE02A
/** @brief Key code: Num Lock / */
#define KEY_RAW_NUMSL   0xE035
/** @brief Key code: Print screen repeat code (total code = E02AE037 */
#define KEY_RAW_PRTSCR  0xE037
/** @brief Key Code:  AltGr*/
#define	KEY_RAW_ALTGR	0xE038
/** @brief Key code: Left win */
#define KEY_RAW_LWIN    0xE05B
/** @brief Key code: Right win */
#define KEY_RAW_RWIN    0xE05C
/** @brief Key code: Menu */
#define KEY_RAW_MENU    0xE05D


/** @brief Flag: Caps Lock is on */
#define KBD_BUCKY_CAPS      0x1000000
/** @brief Flag: Num Lock is on */
#define KBD_BUCKY_NUM       0x2000000
/** @brief Flag: Scroll Lock is on */
#define KBD_BUCKY_SCRL      0x4000000

/* This is a standard
 * Keyboard Map struct.
 * We have different pointers
 * for different states (normal, shift, ...)
 */
/** @brief Keyboard map structure */
typedef struct KeyboardMap
{
    /** @brief Keyboard Map normal keys */
    unsigned long normal;
    /** @brief Keyboard Map shift keys */
    unsigned long shift;
    /** @brief Keyboard Map Control+Alt keys */
    unsigned long altgr;
}KeyboardMap;


/* This is the standard map loaded for PCF
 * It is a Portugal Map so we have a lot of special keys
 *
 * NOTE: Many keys may have a *stupid* work because
 * are not well implemeted. To corect in future
 *
 * Made by: Miguel Abreu
 */
/** @brief Keyboard Portuguese Map. Loaded in default */
KeyboardMap KbPTmap[128] =
{
    // Normal     // Shift       // Control + Alt
    {   0,           0,           0         }, // 0
    {  (KEY_ESC | KB_SPECIAL),    (KEY_ESC | KB_SPECIAL),     0         }, // 1 // Escape
    {  '1',          '!',         0         }, // 2 // !
    {  '2',          '"',         '@'       }, // 3
    {  '3',          '#',         '£'       }, // 4
    {  '4',          '$',         '§'       }, // 5
    {  '5',          '%',         0         }, // 6
    {  '6',          '&',         0         }, // 7
    {  '7',          '/',         '{'       }, // 8
    {  '8',          '(',         '['       }, // 9
    {  '9',          ')',         ']'       }, // 0x0A / 10
    {  '0',          '=',         '}'       }, // 0x0B / 11
    {  '\'',         '?',         0         }, // 0x0C / 12 // '
    {  '<',          '>',         0         }, // ??????? // 0x0D / 13 // «/ »
    {  '\b',         '\b',        0         }, // 0x0E / 14 // Backspace
    {  '\t',         '\t',        0         }, // 0x0F / 15 // Tab
    {  'q',          'Q',         0         }, // 0x10 / 16
    {  'w',          'W',         0         }, // 0x11 / 17
    {  'e',          'E',         0         }, // 0x12 / 18
    {  'r',          'R',         0         }, // 0x13 / 19
    {  't',          'T',         0         }, // 0x14 / 20
    {  'y',          'Y',         0         }, // 0x15 / 21
    {  'u',          'U',         0         }, // 0x16 / 22
    {  'i',          'I',         0         }, // 0x17 / 23
    {  'o',          'O',         0         }, // 0x18 / 24
    {  'p',          'P',         0         }, // 0x19 / 25
    {  '+',          '*',         0         }, // 0x1A / 26 // + / *
    {  '´',          '`',         0         }, // 0x1B / 27 // ´ / `
    {  '\n',         '\n',        0         }, // 0x1C / 28 // Enter / Return
    {  0,            0,           0         }, // 0x1D / 29 // LeftControl Control handled by Keyboard Handler
    {  'a',          'A',         0         }, // 0x1E / 30
    {  's',          'S',         0         }, // 0x1F / 31
    {  'd',          'D',         0         }, // 0x20 / 32
    {  'f',          'F',         0         }, // 0x21 / 33
    {  'g',          'G',         0         }, // 0x22 / 34
    {  'h',          'H',         0         }, // 0x23 / 35
    {  'j',          'J',         0         }, // 0x24 / 36
    {  'k',          'K',         0         }, // 0x25 / 37
    {  'l',          'L',         0         }, // 0x26 / 38
    {  'ç',          'Ç',         0         }, // 0x27 / 39 // ç
    {  'º',          'ª',         0         }, // 0x28 / 40 // º // ª
    {  '\\',         '|',         0         }, // ?????????? // 0x29 / 41
    {  0,            0,           0         }, // 0x2A / 42 // Left Shift handled by Keyboard Handler
    {  '~',          '^',         0         }, // ?????????? // 0x2B / 43 //
    {  'z',          'Z',         0         }, // 0x2C / 44
    {  'x',          'X',         0         }, // 0x2D / 45
    {  'c',          'C',         0         }, // 0x2E / 46
    {  'v',          'V',         0         }, // 0x2F / 47
    {  'b',          'B',         0         }, // 0x30 / 48
    {  'n',          'N',         0         }, // 0x31 / 49
    {  'm',          'M',         0         }, // 0x32 / 50
    {  ',',          ';',         0         }, // 0x33 / 51
    {  '.',          ':',         0         }, // 0x34 / 52
    {  '-',          '_',         0         }, // 0x35 / 53
    {  0,            0,           0         }, // 0x36 / 54 // Right Shift handled by Keyboard Handler
    {  '*',          '*',         0         }, // 0x37 / 55 // Num -> *
    {  0,            0,           0         }, // 0x38 / 56 // Left Alt handled by Keyboard Handler
    {  ' ',          ' ',         0         }, // 0x39 / 57 // Space
    {  0,            0,           0         }, // 0x3A / 58 // Caps Lock handled by Keyboard Handler
    {  (KEY_F1 | KB_SPECIAL),       (KEY_F1 | KB_SPECIAL),      0         }, // 0x3B / 59 // F1
    {  (KEY_F2 | KB_SPECIAL),       (KEY_F2 | KB_SPECIAL),      0         }, // 0x3C / 60 // F2
    {  (KEY_F3 | KB_SPECIAL),       (KEY_F3 | KB_SPECIAL),      0         }, // 0x3D / 61 // F3
    {  (KEY_F4 | KB_SPECIAL),       (KEY_F4 | KB_SPECIAL),      0         }, // 0x3E / 62 // F4
    {  (KEY_F5 | KB_SPECIAL),       (KEY_F5 | KB_SPECIAL),      0         }, // 0x3F / 63 // F5
    {  (KEY_F6 | KB_SPECIAL),       (KEY_F6 | KB_SPECIAL),      0         }, // 0x40 / 64 // F6
    {  (KEY_F7 | KB_SPECIAL),       (KEY_F7 | KB_SPECIAL),      0         }, // 0x41 / 65 // F7
    {  (KEY_F8 | KB_SPECIAL),       (KEY_F8 | KB_SPECIAL),      0         }, // 0x42 / 66 // F8
    {  (KEY_F9 | KB_SPECIAL),       (KEY_F9 | KB_SPECIAL),      0         }, // 0x43 / 67 // F9
    {  (KEY_F10 | KB_SPECIAL),      (KEY_F10 | KB_SPECIAL),     0         }, // 0x44 / 68 // F10
    {  0,            0,           0         }, // 0x45 / 69 // Num Lock handled by Keyboard Handler
    {  0,            0,           0         }, // 0x46 / 70 // Scroll Lock handled by Keyboard Handler
    {  (KEY_HOME | KB_SPECIAL),     (KEY_HOME | KB_SPECIAL),    0         }, // 0x47 / 71 // Num -> Home
    {  (KEY_UPARROW | KB_SPECIAL),  (KEY_UPARROW | KB_SPECIAL), 0         }, // 0x48 / 72 // Num -> Up Arrow
    {  (KEY_PAGEUP | KB_SPECIAL),   (KEY_PAGEUP | KB_SPECIAL),  0         }, // 0x49 / 73 // Num -> Page Up
    {  '-',          '-',         0         }, // 0x4A / 74 // Num -> -
    {  (KEY_LEFTARROW | KB_SPECIAL), (KEY_LEFTARROW | KB_SPECIAL),0        }, // 0x4B / 75 // Num -> Left Arrow
    {  0,            0,           0         }, // 0x4C / 76 // Num -> 5
    {  (KEY_RIGHTARROW | KB_SPECIAL), (KEY_RIGHTARROW | KB_SPECIAL) ,0     }, // 0x4D / 77 // Num -> Right Arrow
    {  '+',          '+',         0         }, // 0x4E / 78 // Num -> +
    {  (KEY_END | KB_SPECIAL),      (KEY_END | KB_SPECIAL),     0         }, // 0x4F / 79 // Num -> End
    {  (KEY_DOWNARROW | KB_SPECIAL),(KEY_DOWNARROW | KB_SPECIAL),0        }, // 0x50 / 80 // Num -> Down Arrow
    {  (KEY_PAGEDOWN | KB_SPECIAL), (KEY_PAGEDOWN | KB_SPECIAL),0         }, // 0x51 / 81 // Num -> Page Down
    {  (KEY_INSERT | KB_SPECIAL),   (KEY_INSERT | KB_SPECIAL),  0         }, // 0x52 / 82 // Num -> Insert
    {  (KEY_DELETE | KB_SPECIAL),   (KEY_DELETE | KB_SPECIAL),  0         }, // 0x53 / 83 // Num -> Delete
    {  0,            0,           0         }, // 0x54 / 84
    {  0,            0,           0         }, // 0x55 / 85
    {  0,            0,           0         }, // 0x56 / 86
    {  (KEY_F11 | KB_SPECIAL),    (KEY_F11 | KB_SPECIAL),     0         }, // 0x57 / 87 // F11
    {  (KEY_F12 | KB_SPECIAL),    (KEY_F12 | KB_SPECIAL),     0         }, // 0x58 / 88 // F12
    {  0,            0,           0         }, // 0x59 / 89
    {  0,            0,           0         }, // 0x5A / 90
    {  0,            0,           0         }, // 0x5B / 91 
    {  0,            0,           0         }, // 0x5C / 92 
    {  0,            0,           0         }, // 0x5D / 93
};
