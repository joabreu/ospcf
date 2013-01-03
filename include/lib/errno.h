/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\errno.h
    @brief Error numbers and access to error reporting
*/
#ifndef INCLUDE_LIB_ERRNO_H
#define INCLUDE_LIB_ERRNO_H

#include <sys\types.h>

/** @brief Error number: Operation completed successfuly */
#define ESUCCESS      0
/** @brief Error number: Operation not permitted */
#define EPERM         1
/** @brief Error number: No such file or directory */
#define ENOENT        2
/** @brief Error number: No such process */
#define ESRCH         3
/** @brief Error number: Interrupted system call */
#define EINTR         4
/** @brief Error number: Input/Output error */
#define EIO           5
/** @brief Error number: No such device or address */
#define ENXIO         6
/** @brief Error number: Argument list too long */
#define E2BIG         7
/** @brief Error number: Exec format error */
#define ENOEXEC       8
/** @brief Error number: Bad file descriptor */
#define EBADF         9
/** @brief Error number: No child processes */
#define ECHILD        10
/** @brief Error number: Resource temporarily unavailable */
#define EAGAIN        11
/** @brief Error number: Can not allocate memory */
#define ENOMEM        12
/** @brief Error number: Permission denied */
#define EACCESS       13
/** @brief Error number: Bad address */
#define EFAULT        14
/** @brief Error number: Block device required */
#define ENOTBLK       15
/** @brief Error number: Device or resource busy */
#define EBUSY         16
/** @brief Error number: File exists */
#define EEXIST        17
/** @brief Error number: Invalid cross-device link */
#define EXDEV         18
/** @brief Error number: No such device */
#define ENODEV        19
/** @brief Error number: Not a directory */
#define ENOTDIR       20
/** @brief Error number: Is a directory */
#define EISDIR        21
/** @brief Error number: Invalid argument */
#define EINVAL        22
/** @brief Error number: Too many open files in system */
#define ENFILE        23
/** @brief Error number: Too many open files */
#define EMFILE        24
/** @brief Error number: Inappropriate I/O control operation */
#define ENOTTY        25
/** @brief Error number: Text file busy */
#define ETXTBUSY      26
/** @brief Error number: File too large */
#define EFBIG         27
/** @brief Error number: No space left on device */
#define ENOSPC        28
/** @brief Error number: Illegal seek */
#define ESPIPE        29
/** @brief Error number: Read-only file system */
#define EROFS         30
/** @brief Error number: Too many links */
#define EMLINK        31
/** @brief Error number: Broken pipe */
#define EPIPE         32
/** @brief Error number: Domain error */
#define EDOM          33
/** @brief Error number: Result too large */
#define ERANGE        34

/** @brief Error number: Not supported */
#define ENOTSUP       134

/** @brief PCF Internal Error number: Entry already filled */
#define EENTRYFLL       200
/** @brief PCF Internal Error number: Init first */
#define EINITFRST       201
/** @brief PCF Internal Error number: Init failed */
#define EIFAIL          202

/** @fn int *_geterrno()
    @brief Return current process last error
    @ingroup libs_libc_errno 
    @return Error number pointer
*/
int *_geterrno();
/** @brief Get Last error number */
#define errno  (*_geterrno())

#endif
