/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\stdio.h
    @brief Standard input/output functions
*/
#ifndef INCLUDE_LIB_STDIO_H
#define INCLUDE_LIB_STDIO_H

#include <stdarg.h>
#include <sys\types.h>

// iobuf struct flags
/** @brief Currently reading */
#define	_IOREAD	1
/** @brief Currently writing */
#define	_IOWRT	2
/** @brief Opened as 'r+w' */
#define	_IORW	0x0080

/** @brief Buffer size */
#define	BUFSIZ	512

/** @brief Error number returned by many functions of this library */
#define EOF      (-1)
/** @brief Seek function: Seek from beggining */
#define SEEK_SET	(0)
/** @brief Seek function: Seek from current position */
#define	SEEK_CUR	(1)
/** @brief Seek function: Seek from end */
#define SEEK_END	(2)

/** @brief A standard FILE struct */
typedef struct _iobuf
{
	/** @brief File internal id */
    int fd;
    /** @brief File attributes */
	int flags;
	/** @brief File size */
	int size;
	/** @brief Buffer pointer */
	char *buffer;
	/** @brief Buffer position */
	int bufpos;
	/** @brief Current buffer size */
	int bufsize;
	/** @brief Buffer maximum lenght */
	int buflen;
	/** @brief Pointer to os file struct */
	char *internal;
} FILE;

/** @brief Standard input file number */
#define STDIN_FD      0
/** @brief Standard output file number */
#define STDOUT_FD     1
/** @brief Standard error file number */
#define STDERR_FD     2

/** @fn int printf(const char *fmt, ...)
    @brief Outputs string into STDOUT
    @ingroup libs_libc_stdio 
    @param fmt String unparsed
    @return Parsed string size
*/
int printf(const char *fmt, ...);
/** @fn int sprintf(char *buffer, const char *fmt, ...)
    @brief Outputs string into buffer
    @ingroup libs_libc_stdio 
    @param buffer String destination
    @param fmt String unparsed
    @return Parsed string size
*/
int sprintf(char *buffer, const char *fmt, ...);
/** @fn int vsprintf(char *buffer, const char *fmt, va_list args)
    @brief Outputs string into buffer supply arguments
    @ingroup libs_libc_stdio 
    @param buffer String destination
    @param fmt String unparsed
    @param args Arguments list
    @return Parsed string size
*/
int vsprintf(char *buffer, const char *fmt, va_list args);
/** @fn int fgetc(FILE *f)
    @brief Get a single char from file f
    @ingroup libs_libc_stdio 
    @param f File struct
    @return Char read or EOF
*/
int fgetc(FILE *f);
/** @fn char *fgets(char *str, int size, FILE *f)
    @brief Get a single line from file f with size size
    @ingroup libs_libc_stdio 
    @param str String destination
    @param size Size of line
    @param f File struct
    @return Pointer to string location (=str) or NULL
*/
char *fgets(char *str, int size, FILE *f);
/** @fn int fputc(int c, FILE *f)
    @brief Writes a single char into file f
    @ingroup libs_libc_stdio 
    @param c Char
    @param f File struct
    @return ESUCCESS or EOF
*/
int fputc(int c, FILE *f);
/** @fn int fputs(const char *str, FILE *f)
    @brief Writes a single line into file f
    @ingroup libs_libc_stdio 
    @param str String
    @param f File struct
    @return ESUCCESS or EOF
*/
int fputs(const char *str, FILE *f);
/** @fn char *gets(char *str)
    @brief Reads a single line from STDIN
    @ingroup libs_libc_stdio 
    @param str Writes line to this location
    @return String end pointer
*/
char *gets(char *str);
/** @fn int puts(const char *str)
    @brief Writes a single line to STDOUT
    @ingroup libs_libc_stdio 
    @param str String
    @return ESUCCESS or EOF
*/
int puts(const char *str);
/** @fn int getc(FILE *f)
    @brief Reads a single char from file f (=fgetc())
    @ingroup libs_libc_stdio 
    @param f File struct
    @return Char read or EOF
*/
int getc(FILE *f);
/** @fn int putc(int c, FILE *f)
    @brief Writes a single char to file f (=fputc())
    @ingroup libs_libc_stdio 
    @param c Char
    @param f File struct
    @return ESUCCESS or EOF
*/
int putc(int c, FILE *f);
/** @fn int getchar()
    @brief Reads a single char from STDIN
    @ingroup libs_libc_stdio 
    @return Char
*/
int getchar();
/** @fn int putchar(int c)
    @brief Writes a single char to STDOUT
    @ingroup libs_libc_stdio 
    @param c Char
    @return Bytes written
*/
int putchar(int c);
/** @fn int do_printf(const char *fmt, va_list args, int (*fn)(unsigned c, void **helper), void *ptr)
    @brief Parse a string and output it using fn function
    @ingroup libs_libc_stdio 
    @param fmt String to parse
    @param args Arguments list
    @param fn Parsed string is output here char by char
    @param ptr NULL
    @return Parsed string size
*/
int do_printf(const char *fmt, va_list args, int (*fn)(unsigned c, void **helper), void *ptr);

#endif
