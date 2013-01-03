/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\lib\os.h
    @brief OS specific functions
*/
#ifndef INCLUDE_LIB_OS_H
#define INCLUDE_LIB_OS_H

/** @fn int exit(int errorcode)
    @brief Kills current thread. It this is the process main thread, kill whole process
    @ingroup libs_libc_os
    @param errorcode Error code
    @return Error code
*/
int exit(int errorcode);
/** @fn int read(int fd, char *buffer, unsigned int len)
    @brief Reads len bytes from file fd and puts them into buffer
    @ingroup libs_libc_os
    @param fd File id
    @param buffer Ptr to write read data
    @param len Len bytes to read
    @return Bytes read or negative error code
*/
int read(int fd, char *buffer, unsigned int len);
/** @fn int write(int fd, char *buffer, unsigned int len)
    @brief Writes len bytes to file fd
    @ingroup libs_libc_os
    @param fd File id
    @param buffer Ptr to read the data from
    @param len Len bytes to write
    @return Bytes written or negative error code
*/
int write(int fd, char *buffer, unsigned int len);

#endif
