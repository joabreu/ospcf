/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\fs\vfs.h
    @brief Virtual File System
*/
#ifndef INCLUDE_SYSTEM_KERNEL_FS_VFS_H
#define INCLUDE_SYSTEM_KERNEL_FS_VFS_H

#include <sys\stat.h>

/** @brief The VNODE is a directory */
#define VFS_DIRECTORY 0x01
/** @brief The VNODE is a file */
#define VFS_FILE      0x02
/** @brief The VNODE is a device */
#define VFS_DEVICE    0x03

/** @brief Maximum size of a filename */
#define MAX_FILENAME 256
/** @brief Maximum number of simultaneus files open
    @todo Remove, use queue */
#define MAX_FILE 32

/** @brief File operation mode: Read only */
#define O_RDONLY	00
/** @brief File operation mode: Write only */
#define O_WRONLY	01
/** @brief File operation mode: Read/Write */
#define O_RDWR		02
/** @brief File operation mode: Create if not existent */
#define O_CREAT		0x200
/** @brief File operation mode: 
    @todo Documentate */
#define O_EXCL		0x800
/** @brief File operation mode: 
    @todo Documentate */
#define O_TRUNC		01000
/** @brief File operation mode: 
    @todo Documentate */ 
#define O_NONBLOCK	04000

#ifndef SEEK_START
/** @brief File seek to position: Begin of file as offset*/
#define SEEK_START 0
#endif
#ifndef SEEK_CURRENT
/** @brief File seek to position: Current position as offset */
#define SEEK_CURRENT 1
#endif

/** @brief File seek to position: set offset as file position */
#define SEEK_SET 0
/** @brief File seek to position: seek from current file offset */
#define SEEK_CUR 1
/** @brief File seek to position: not implemented
    @todo Implement */
#define SEEK_END 2

/** @brief Standard filldir() function */
typedef int (*filldir_t)     (void *_buff, const char *name, int namelen, long offset, unsigned short inode);

/** @brief File struct */
typedef struct _FileStruct
{
	/** @brief File iDentiFication */
    int fd;
    /** @brief Device ID of this file. High 16bits major/Low 16bits minor */
    unsigned long device;
    /** @brief Used */
	char used;
	/** @brief Position in writing / reading */
	long long pos;
	/** @brief Current file flags */
	unsigned long flags;
	/** @brief File operations */
	struct FileOperationsStruct *fops;
	/** @brief File vnode */
	struct _VnodeStruct *vnode;
    /** @brief For internal use of VFS */  
	void *data;
} FileStruct;

/** @brief VNODE struct */
typedef struct _VnodeStruct
{	
	/** @brief Next vnode */
	struct _VnodeStruct *next;
        /** @brief Parent device ID */
  	unsigned long device;
        /** @brief Times used */
	unsigned long used;
        /** @brief Mode (file, directory, etc...) */
	unsigned long mode;
        /** @brief Vnode ID */
	unsigned long id;
	/** @brief Total size */
	unsigned long long size;
	/** @brief User identification */
	unsigned long uid;
	/** @brief Group identification */
	unsigned long gid;
	/** @brief Mount covered vnode, '..' case */
	struct _VnodeStruct *covered_by;
	/** @brief Mount point -> mount root vnode */
	struct MountStruct *mount;
	/** @brief Possible file operations */
	struct FileOperationsStruct *fops;
	/** @brief File system of this vnode */
	struct FilesystemStruct *fs;
	/** @brief Last accessed time */
	unsigned long accessTime;
	/** @brief Creation time */
	unsigned long createTime;
	/** @brief Last Modified time */
	unsigned long modifyTime;
	/** @brief Internal use */
	char *data;
} VnodeStruct;

/** @brief Mount point struct */
typedef struct MountStruct
{
	/** @brief Device ID mounting */
	unsigned long device;
	/** @brief File system of this mount point */
	struct FilesystemStruct *fs;
	/** @brief Internal Data */
	void *fsInfo;
	/** @brief Mount point string */
	char *mountpoint;
	/** @brief Root vnode of this mount point */
	VnodeStruct *root_vnode;
	/** @brief Current vnode requesting the mount */
	VnodeStruct *mount_cover;
	/** @brief Vnode table */
	VnodeStruct *vnode_table;
	/** @brief Next mount point in a list */
	struct MountStruct *next;
} MountStruct;

/** @brief Possibly file operations struct */
typedef struct FileOperationsStruct
{
	/** @brief Structure of a file seek function
	    @ingroup sys_kernel_fs 
	    @param file File to seek
	    @param offset Offset to seek
	    @param from Seek from: beggining, current position
        @return Error code or file new position
    */
    int (* seek)   (struct _FileStruct *file, long offset, int from);
	/** @brief Read len bytes from file
	    @ingroup sys_kernel_fs 
	    @param file File to read
	    @param buffer Data destination
	    @param len Count bytes to read
        @return Error code or count of bytes read
    */
    int (* read)   (struct _FileStruct *file, char *buffer, int len);
	/** @brief Write len bytes to file
	    @ingroup sys_kernel_fs 
	    @param file File to write
	    @param buffer Data origin
	    @param len Count bytes to write
        @return Error code or count of bytes wrote
    */
    int (* write)  (struct _FileStruct *file, char *buffer, int len);
	/** @brief Send an IO command to device
	    @ingroup sys_kernel_fs 
	    @param file File that is assigned to device
	    @param cmd Command specification
	    @param arg Command data
        @return Error code or command result
    */
    int (* ioctl)  (struct _FileStruct *file, unsigned long cmd, unsigned long arg);
	/** @brief Marks a file as opened
	    @ingroup sys_kernel_fs 
	    @param file File to open
        @return TRUE or FALSE
    */
    int (* open)   (struct _FileStruct *file);
	/** @brief Marks a file as closed
	    @ingroup sys_kernel_fs 
	    @param file File to close
        @return TRUE or FALSE
    */
    int (* close)  (struct _FileStruct *file);
} FileOperationsStruct;

/** @brief File system struct */
typedef struct FilesystemStruct
{
	/** @brief Filesystem name */
    char *name;
    /** @brief Size of filesystem blocks in bytes */
	unsigned long blocksize;
	/** @brief Number of blocks */
	unsigned long blockcount;
	/** @brief File system operations */
	struct FsCallsStruct *fscalls;
	/** @brief Linked list */
	struct FilesystemStruct *next;
} FilesystemStruct;

/** @brief Possibly file system operations struct */
typedef struct FsCallsStruct
{
	/** @brief Find a vnode with a specific name
	    @ingroup sys_kernel_fs 
	    @param vnode Searched vnode placed here (if found)
	    @param src Current vnode, search inside this
	    @param name Vnode name to find
        @return TRUE or FALSE
    */
    int (* FindVnode)       (struct _VnodeStruct **vnode, struct _VnodeStruct *src, char *name);
	/** @brief Mount the file system
	    @ingroup sys_kernel_fs 
	    @param mount Mount point struct
	    @param device Device ID
	    @param vnode Root vnode provided by the FS
        @return TRUE or FALSE
    */
    int (* mount)           (struct MountStruct *mount, int device, struct _VnodeStruct **vnode);
	/** @brief Gets the pointer to a vnode structure with selected ID
	    @ingroup sys_kernel_fs 
	    @param vnid Vnode ID to find
	    @param data Pointer to vnode stored here
        @return TRUE
    */
    int (* GetVnodeData)    (int vnid, char **data);
	/** @brief Creates a new directory
	    @ingroup sys_kernel_fs 
	    @param parentVnode Current Vnode
	    @param name New directory name
        @return Error code
    */
    int (* MakeDirectory)   (struct _VnodeStruct *parentVnode, char *name);
	/** @brief Reads the contents of a directory
	    @ingroup sys_kernel_fs 
	    @param file Pointer to directory
	    @param dirent Directory entry
	    @param filldir Fill dir function pointer
        @return TRUE or FALSE
    */
    int (* ReadDirectory)   (struct _FileStruct *file, void *dirent, filldir_t filldir);	
	/** @brief Creates a new file
	    @ingroup sys_kernel_fs 
	    @param parentVnode Current Vnode
	    @param name New file name
	    @param mode New file attributes
        @return Error code
    */
    int (* create)          (struct _VnodeStruct *parentVnode, char *name, unsigned short mode);
	/** @brief Renames a file
	    @ingroup sys_kernel_fs 
	    @param vnode File to rename
	    @param name New file name
        @return Error code
    */
	int (* rename)          (struct _VnodeStruct *vnode, char *name);
	/** @brief Removes a file
	    @ingroup sys_kernel_fs 
	    @param vnode File to remove
        @return Error code
    */
	int (* remove)          (struct _VnodeStruct *vnode);
	/** @brief Gets the information (stats) about a file
	    @ingroup sys_kernel_fs 
	    @param file File to get the stats
	    @param buf Stats stored here
        @return Error code
    */
	int (* stat)            (struct _FileStruct *file, struct StatStruct *buf);
} FsCallsStruct;

/*
void AddMount(MountStruct *mount);
void RemoveMount(MountStruct *mount); // IMPLEMENT !
int VnodeAddToMount(MountStruct *mount, VnodeStruct *vnode);
int VnodeAddToMount(MountStruct *mount, VnodeStruct *vnode);
VnodeStruct *VfsGetRootVnode();*/


















/** @fn int VfsRead(int fd, char *buffer, unsigned int len)
    @brief Read len bytes from file fd
    @ingroup sys_kernel_fs
    @param fd File internal id
    @param buffer Data destination
    @param len Bytes
    @return Bytes read or negative error code
*/
int VfsRead(int fd, char *buffer, unsigned int len);
/** @fn int VfsWrite(int fd, char *buffer, unsigned int len)
    @brief Write len bytes to file fd
    @ingroup sys_kernel_fs
    @param fd File internal id
    @param buffer Data origin
    @param len Bytes
    @return Bytes written or negative error code
*/
int VfsWrite(int fd, char *buffer, unsigned int len);

#endif
