/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\fs\dirent.h
    @brief Directory entry
*/
#ifndef INCLUDE_SYSTEM_KERNEL_FS_DIRENT_H
#define INCLUDE_SYSTEM_KERNEL_FS_DIRENT_H
/*
  Copyright 2007
  International Business Machines Corporation,
  Sony Computer Entertainment, Incorporated,
  Toshiba Corporation,

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
    * Neither the names of the copyright holders nor the names of their
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Directory Entry */
struct dirent
{
    /** @brief Directory ID (INO)*/
    unsigned long d_ino;
    /** @brief Offset */
    long d_off;
    /** @brief Recorded len */
    unsigned short int d_reclen;
    /** @brief Type */
    unsigned char d_type;
    /** @brief Name */
    char d_name[256];
};
/** @brief Directory Struct */
typedef struct
{
    /** @brief Directory file */
    int dd_fd;
    /** @brief Position in buffer */
    int dd_loc;
    /** @brief Directory seek */
    int dd_seek;
    /** @brief Buffer */
    char *dd_buf;
    /** @brief Buffer length */
    int dd_len;
    /** @brief Amount of data in buffer */
    int dd_size;
} DIR;

#ifdef __cplusplus
}
#endif
#endif
