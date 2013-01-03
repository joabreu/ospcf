/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\sys\stat.h
    @brief File status
*/
#ifndef INCLUDE_SYS_STAT_H
#define INCLUDE_SYS_STAT_H

/** @brief Flag: 
    @todo Documentate */
#define S_IFMT	00170000
/** @brief Flag: Is a socket */
#define S_IFSOCK 0140000
/** @brief Flag: 
    @todo Documentate */
#define S_IFLNK  0120000
/** @brief Flag: 
    @todo Documentate */
#define S_IFREG  0100000
/** @brief Flag: 
    @todo Documentate */
#define S_IFBLK  0060000
/** @brief Flag: Is a dir */
#define S_IFDIR  0040000
/** @brief Flag: 
    @todo Documentate */
#define S_IFCHR  0020000
/** @brief Flag: 
    @todo Documentate */
#define S_IFIFO  0010000
/** @brief Flag: 
    @todo Documentate */
#define S_ISUID  0004000
/** @brief Flag: 
    @todo Documentate */
#define S_ISGID  0002000
/** @brief Flag: 
    @todo Documentate */
#define S_ISVTX  0001000

/** @brief Cheks if flag S_IFLNK is present */
#define S_ISLNK(_x_) (((_x_) & S_IFMT) == S_IFLNK)
/** @brief Cheks if flag S_IFREG is present */
#define S_ISREG(_x_) (((_x_) & S_IFMT) == S_IFREG)
/** @brief Cheks if flag S_IFDIR is present */
#define S_ISDIR(_x_) (((_x_) & S_IFMT) == S_IFDIR)
/** @brief Cheks if flag S_IFCHR is present */
#define S_ISCHR(_x_) (((_x_) & S_IFMT) == S_IFCHR)
/** @brief Cheks if flag S_IFBLK is present */
#define S_ISBLK(_x_) (((_x_) & S_IFMT) == S_IFBLK)
/** @brief Cheks if flag S_IFIFO is present */
#define S_ISIFO(_x_) (((_x_) & S_IFMT) == S_IFIFO)
/** @brief Cheks if flag S_IFSOCK is present */
#define S_ISSOCK(_x_)(((_x_) & S_IFMT) == S_IFSOCK)

/** @brief Flag: 
    @todo Documentate */
#define S_IRWXU 00700
/** @brief Flag: 
    @todo Documentate */
#define S_IRUSR 00400
/** @brief Flag: 
    @todo Documentate */
#define S_IWUSR 00200
/** @brief Flag: 
    @todo Documentate */
#define S_IXUSR 00100
/** @brief Flag: 
    @todo Documentate */
#define S_IRWXG 00070
/** @brief Flag: 
    @todo Documentate */
#define S_IRGRP 00040
/** @brief Flag: 
    @todo Documentate */
#define S_IWGRP 00020
/** @brief Flag: 
    @todo Documentate */
#define S_IXGRP 00010
/** @brief Flag: 
    @todo Documentate */
#define S_IRWXO 00007
/** @brief Flag: 
    @todo Documentate */
#define S_IROTH 00004
/** @brief Flag: 
    @todo Documentate */
#define S_IWOTH 00002
/** @brief Flag: 
    @todo Documentate */
#define S_IXOTH 00001

/** @brief A file status struct */
struct StatStruct
{
	/** @brief Device */
	unsigned short st_dev;
	/** @brief INO */
	unsigned short st_ino;
	/** @brief Mode */
	unsigned short st_mode;
	/** @brief N link */
	unsigned short st_nlink;
	/** @brief UID */
	unsigned short st_uid;
	/** @brief GID */
	unsigned short st_gid;
	/** @brief RDEV */
	unsigned short st_rdev;
	/** @brief Size */
	long st_size;
	/** @brief A time */
	long st_atime;
	/** @brief Spare 1 */
	long st_spare1;
	/** @brief M time */
	long st_mtime;
	/** @brief Spare 2 */
	long st_spare2;
	/** @brief C time */
	long st_ctime;
	/** @brief Spare 3 */
	long st_spare3;
	/** @brief Block size */
	long st_blksize;
	/** @brief Blocks */
	long st_blocks;
	/** @brief Spare 4 */
	long st_spare4[2];
};

#endif
