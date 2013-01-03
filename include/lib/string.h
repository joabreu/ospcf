/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\string.h
    @brief Memory and string functions
*/
#ifndef INCLUDE_LIB_STRING_H
#define INCLUDE_LIB_STRING_H

/** @fn void *memcpy(void *dst_ptr, const void *src_ptr, unsigned len)
    @brief Copies len bytes from src_ptr to dst_ptr
    @ingroup libs_libc_string 
    @param dst_ptr Data destination
    @param src_ptr Data origin
    @param len Bytes to copy
    @return Data destination pointer
*/
void *memcpy(void *dst_ptr, const void *src_ptr, unsigned len);
/** @fn void *memmove(void *dst_ptr, const void *src_ptr, unsigned len)
    @brief Moves len bytes from src_ptr to dst_ptr
    @ingroup libs_libc_string 
    @param dst_ptr Data destination
    @param src_ptr Data origin
    @param len Bytes to move
    @return Data destination pointer
*/
void *memmove(void *dst_ptr, const void *src_ptr, unsigned len);
/** @fn void *memset(void *dst, int val, unsigned count)
    @brief Set count times the value val
    @ingroup libs_libc_string 
    @param dst Data destination
    @param val Value
    @param count Number of bytes to set
    @return Data destination pointer
*/
void *memset(void *dst, int val, unsigned count);
/** @fn char *strcat(char *str, const char *append)
    @brief Append string append to string str
    @ingroup libs_libc_string 
    @param str Data destination
    @param append Data to append
    @return Data destination pointer
*/
char *strcat(char *str, const char *append);
/** @fn char *strchr(const char *str, int c)
    @brief Search for char c in string str. Returns first occurence
    @ingroup libs_libc_string 
    @param str Data to search
    @param c Char
    @return Char position or 0
*/
char *strchr(const char *str, int c);
/** @fn int strcmp(const char *s1, const char *s2)
    @brief Return number of diferent bytes from s1 to s2
    @ingroup libs_libc_string 
    @param s1 String
    @param s2 String
    @return Diferent bytes
*/
int strcmp(const char *s1, const char *s2);
/** @fn char *strcpy(char *to, const char *from)
    @brief Copy string 'from' to dest 'to'
    @ingroup libs_libc_string 
    @param to Data destination
    @param from Data origin
    @return Data destination pointer
*/
char *strcpy(char *to, const char *from);
/** @fn unsigned strlen(const char *str)
    @brief Returns size of string str
    @ingroup libs_libc_string 
    @param str String
    @return String size
*/
unsigned strlen(const char *str);
/** @fn char *strncat(char *dst, const char *src, unsigned n)
    @brief Append n chars from string src to string dst
    @ingroup libs_libc_string 
    @param dst Data destination
    @param src Data origin
    @param n Chars to append
    @return Data destination pointer
*/
char *strncat(char *dst, const char *src, unsigned n);
/** @fn int strncmp(const char *s1, const char *s2, unsigned n)
    @brief See strcmp() uses n to restrite comparation
    @ingroup libs_libc_string 
    @param s1 String
    @param s2 String
    @param n Chars to compare
    @return Diference between strings
*/
int strncmp(const char *s1, const char *s2, unsigned n);
/** @fn char *strncpy(char *dst, const char *src, unsigned n)
    @brief Copy n chars from string src to string dst
    @ingroup libs_libc_string 
    @param dst Data destination
    @param src Data origin
    @param n Chars to copy
    @return Data destination pointer
*/
char *strncpy(char *dst, const char *src, unsigned n);
/** @fn char *strrchr(const char *s, int c)
    @brief Search for char c in string str. Returns lst occurence
    @ingroup libs_libc_string 
    @param s Data to search
    @param c Char
    @return Char position or 0
*/
char *strrchr(const char *s, int c);

#endif
