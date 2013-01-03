/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\ctype.h
    @brief Char manipulation functions
*/
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#ifndef INCLUDE_LIB_CTYPE_H
#define INCLUDE_LIB_CTYPE_H

/** @fn int	isalnum(int c)
    @brief Checks if c is an alpha numeric char. isalpha(c) or isdigit(c)
    @ingroup libs_libc_ctype
    @param c Char
    @return true or false
*/
int	isalnum(int c);
/** @fn int	isalpha(int c)
    @brief isupper(c) or islower(c)
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	isalpha(int c);
/** @fn int	iscntrl(int c)
    @brief Is control character. In ASCII, control characters are 0x00 (NUL) to 0x1F (US), and 0x7F (DEL) 
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	iscntrl(int c);
/** @fn int	isdigit(int c)
    @brief Checks if c is a digit
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	isdigit(int c);
/** @fn int	isgraph(int c)
    @brief Is printing character other than space 
    @ingroup libs_libc_ctype
    @param c Char
    @return true or false
*/
int	isgraph(int c);
/** @fn int	islower(int c)
    @brief Checks if c is a lower char
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	islower(int c);
/** @fn int	isprint(int c)
    @brief Checks if it c is a printable char. In ASCII, printing characters are 0x20 (' ') to 0x7E ('~') 
    @ingroup libs_libc_ctype
    @param c Char
    @return true or false
*/
int	isprint(int c);
/** @fn int	ispunct(int c)
    @brief Is printing character other than space, letter, digit
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	ispunct(int c);
/** @fn int	isspace(int c)
    @brief Is space, formfeed, newline, carriage return, tab, vertical tab
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	isspace(int c);
/** @fn int	isupper(int c)
    @brief Checks if c is an upper char
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	isupper(int c);
/** @fn int	isxdigit(int c)
    @brief Checks if c is an hex digit
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int	isxdigit(int c);
/** @fn int	tolower(int c)
    @brief Turns c into lower char
    @ingroup libs_libc_ctype 
    @param c Char
    @return Lower c
*/
int	tolower(int c);
/** @fn int	toupper(int c)
    @brief Turns c into upper char
    @ingroup libs_libc_ctype 
    @param c Char
    @return Upper c
*/
int	toupper(int c);


/** @fn int isascii(int c)
    @brief Checks if c is a valid ascii char
    @ingroup libs_libc_ctype 
    @param c Char
    @return true or false
*/
int isascii(int c);
/** @fn int toascii(int c)
    @brief Turns c into an ascii char
    @ingroup libs_libc_ctype 
    @param c Char
    @return c value in ascii
*/
int toascii(int c);

#endif
