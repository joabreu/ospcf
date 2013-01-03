/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\fs\vfs.c
    @brief Virtual File System
*/
/* IMPORTANT NOTE: Part of this code was ported from "ominOS"
   the original copyright is written above */
/*
 * ominOS - Copywrite (c) 2006, 2007 Anthony Lineberry
 * http://ominos.sourceforge.net
*/
#include <errno.h>
#include <kernel\debug.h>
#include <kernel\fs\vfs.h>

// If defined debug will be outputed
#define DEBUG_VFS
// Debug Message
#ifdef DEBUG_VFS
  /** @brief If debug is selected print a message using the standard DEBUG(...) function */
  #define DM(Msg...) DEBUG(Msg)
#else
  /** @brief Debug is not selected */ 
  #define DM(Msg...) {}
#endif

/** @brief List of file systems installed */
static FilesystemStruct *FsListHead;
/** @brief List of mounted devices */
static MountStruct *MountListHead;
/** @brief Root Vnode */
static VnodeStruct *RootVnode;

void AddMount(MountStruct *mount)
{
	if(MountListHead == 0)
	{
		 // Starting the list
         MountListHead = mount;
		 MountListHead->next = 0;
	}
	// Add mount to beginning of list;
	mount->next = MountListHead;
	MountListHead = mount;
	return;
}
void RemoveMount(MountStruct *mount)
{
    /* Dismount something:
           - disconect all vnodes;
           - search;
           - remove.
    */
}
int VnodeAddToMount(MountStruct *mount, VnodeStruct *vnode)
{
	VnodeStruct *v;

	if(mount == NULL || vnode == NULL)
		 return 0;

	DM("Adding vnode %d (0x%08x) to mount Vnode List", vnode->id, vnode);
	
	if(mount->vnode_table == 0) // The list is empty, add it to the beginning
    {
         mount->vnode_table = vnode;
         //////////////////////////////////////!!!!!!!!!!!!!!!!////////
         v = mount->vnode_table;
         v->next = 0;
         return 1;
         /////////////////////////////////////!!!!!!!!!!!!!!!!!////////
    }
    v = mount->vnode_table;

	while(v->next != 0)
		 v = v->next;

	// Add it to the end of the list
    v->next = vnode;
	v = v->next;
	v->next = 0;
	return 1;
}
VnodeStruct *VfsGetRootVnode()
{
	return RootVnode;
}
















VnodeStruct *Path2Vnode(char *_path)
{
	char *p, *path;
	char *nextp;
	VnodeStruct *currv;
	VnodeStruct *nextv;
	ProcessStruct *process;

	process = GetCurrentProcess();
	path = kstrdup(_path);
	if(path == 0)
		 return 0;

	p = path;
	// are we at the root?
	if(*p == '/')
	{
		 // advance past all slashes if something like ///home/somedir is given
		 while(*++p == '/')
			  ;
		 if(RootVnode == NULL)
         {
			  DEBUG("RootVnode is empty");
              kfree(path);
			  return 0;
		 }
		 currv = RootVnode;
	}
	else
	{
		 // Get current directory
		 if(!process)
         {
			  // No Process running ?!
              DEBUG("No process is running");
              kfree(path);
			  return 0;
		 }
		 currv = process->tcurrent->CurrentDir;
	}
	
	
	while(1)
	{
		 // are we done parsing the path?
		 if(*p == '\0')
			  break;
	
		 // find the next part of the path
		 for(nextp = p+1; *nextp != '\0' && *nextp != '/'; nextp++)
			  ;
		
		 if(*nextp == '/')
		 {
			  *nextp = '\0';
			  do
				   nextp++;
			  while(*nextp == '/');
		 }
		 if(!strcmp(".", p))
		 {
			  p = nextp;
			  continue;
		 }
		 if(strcmp("..", p) == 0 && currv->mount->root_vnode == currv)
		 {
			  if(currv->mount->mount_cover)
			  {
				   nextv = currv->mount->mount_cover;
				   currv = nextv;
			  }
		 }
		 currv->mount->fs->fscalls->FindVnode(&nextv, currv, p);
		 if(nextv == NULL)
		 {
			  DEBUG("Could not lookup vnode for %s from vnode %d", p, currv->id);
			  kfree(path);
			  return 0;
		 }
		 p = nextp;
		 currv = nextv;

		 // did we hit a mount point?
         if(currv->covered_by != NULL)
		 {
			  // if so, we need to be pointing to the rootvnode of the mount
              nextv = currv->covered_by;
              currv = nextv;
		 }
	}
    kfree(path);
	return currv;
}
/** @fn static int InitVnode(VnodeStruct *v)
    @brief Inits a vnode by clearing its structure
    @ingroup sys_kernel_fs 
    @param v Vnode to init
    @return Error Code
    @internal
*/
static int InitVnode(VnodeStruct *v)
{
	memset(v, 0, sizeof(VnodeStruct));
	return ESUCCESS;
}
VnodeStruct *CreateVnode(void)
{
	VnodeStruct *v = (VnodeStruct *)kmalloc(sizeof(VnodeStruct));
	if(v == 0)
		 return 0; // Not enough memory
	InitVnode(v);
	return v;
}
VnodeStruct *VfsPathToDirVnode(char *_path, char *filename)
{
	char *p;
	VnodeStruct *vnode;
	char *path = kstrdup(_path);
	
	memset(filename, 0, MAX_FILENAME);
	// if last char is a slash, change it to NULL char	
	if(path[strlen(path)] == '/')
		 path[strlen(path)] = '\0';

	// find the last occurrence of '/' in the path
	p = strrchr(path, '/');
	if(!p)
	{
		 //path is just a single word with no '/' in it (like "home")
		 strncpy(filename, path, strlen(path));
		 path[0] = '.';
		 path[1] = '\0';	
	}
	else
	{
		 strncpy(filename, p+1, strlen(p));
		 if(p[1] != '\0')
		 {
			  p[1] = '.';
			  p[2] = '\0';
		 }
	}
	vnode = Path2Vnode(path);
	kfree(path);
	if(vnode == 0)
		 return 0; // Vnode is not present
	return vnode;
}
VnodeStruct *VfsGetVnode(MountStruct *mount, int vnid)
{
	VnodeStruct *v;
	v = mount->vnode_table;
	// try to find vnode in list
	while(v != 0)
	{
		 if(v->id == vnid)
			  return v;
		 v = v->next;
	}
	// we didn't find it, so create the vnode;
	v = CreateVnode();
	if(v == 0)
		 return 0; // Could not create vnode
	v->id = vnid;
	v->mount = mount;
	if(v->mount->fs && v->mount->fs->fscalls && v->mount->fs->fscalls->GetVnodeData)
		 v->mount->fs->fscalls->GetVnodeData(vnid, &v->data);
	VnodeAddToMount(mount, v);
	return v;
}
FilesystemStruct *GetFileSystem(char *fstype)
{
	FilesystemStruct *fs = FsListHead;
	while(fs != 0)
	{
		 if(strcmp(fstype, fs->name) == 0)
		 {
			  return fs;
		 }
		 fs = fs->next;
	}
	return 0; // Could not find file system
}
FileOperationsStruct *VfsGetOps(char *name)
{
	VnodeStruct *vnode;
	vnode = Path2Vnode(name);
	if(vnode == 0)
		 return 0; // Vnode not present
	return vnode->fops;
}
int VfsGetDevice(char *name)
{
	VnodeStruct *vnode;
	vnode = Path2Vnode(name);
	if(vnode == 0)
		 return 0; // Device not present
	return (int)vnode->device;
}
int VfsOpen(char *name, unsigned short mode)
{
	int fd; // file descriptor
	FileStruct *file;
	VnodeStruct *vnode;
	ProcessStruct *process = GetCurrentProcess();
	
	if(process == 0)
		 return -ENOPROC;
	if(name == 0)
		 return -EINVAL;

	DEBUG("name:%s, flags:%d", name, mode);
	fd = 0;

	// Linked list
	while(fd < MAX_FILE && process->file[fd])
		 fd++;
	if(fd > MAX_FILE)
		 return -EMFILE;
		
	vnode = Path2Vnode(name);

	if(vnode == 0)
	{
		 if(mode & O_CREAT)
         {
			  // Create the file here
			  VfsCreate(name, mode);
			  vnode = Path2Vnode(name);
			  if(vnode == 0)
              {
				   return -EIO;
			  }
		 }
         else
         {
			  /** @todo check: kfree(file); */
			  return -ENOENT;
		 }
	}
	file = (FileStruct *)kmalloc(sizeof(FileStruct));
	if(file == 0)
		 return -ENOMEM;

	memset(file, 0, sizeof(FileStruct));

	file->vnode = vnode;
	file->device = vnode->device;
	file->fops = vnode->fops;
	file->used = 1;
	file->flags = mode;
	file->pos = 0;
	// Linked list
    process->file[fd] = file;
	if(file->fops && file->fops->open)
	{
		 if(file->fops->open(file) < 0)
         {
			  kfree(file);
			  process->file[fd] = 0;
			  return -EIO;
		 }
	}
    DEBUG("fd:%d, maj:%d, min:%d, fops:0x%08x",fd, 
			DEV_MAJOR(file->device), DEV_MINOR(file->device), file->fops);
	return fd;
}
int VfsClose(int fd)
{
	ProcessStruct *process = GetCurrentProcess();
	FileStruct *file = process->file[fd];
	if(file == 0)
		 return 0;
	DEBUG("fd:%d", fd);
	if(file->fops && file->fops->close)
		 file->fops->close(file);

	kfree(file);
	process->file[fd] = 0;
	return 1;
}
int VfsRead(int fd, char *buffer, int len)
{
	int bytes_read=0;
	ProcessStruct *process = GetCurrentProcess();
	FileStruct *file = process->file[fd];
	if(file == 0)
		 return 0; // Bad File
    DEBUG("fd:%d, buffer:0x%08x, len:%d", fd, buffer, len);
	if(file->fops && file->fops->read)
		bytes_read = file->fops->read(file, buffer, len);
	DEBUG("bytes_read:%d", bytes_read);
	return bytes_read;
}
int VfsWrite(int fd, char *buffer, int len)
{
	ProcessStruct *process = GetCurrentProcess();
	FileStruct *file = process->file[fd];
	if(file == 0)
		 return 0; // Bad File
    DEBUG("fd:%d, buffer:0x%08x, len:%d", fd, buffer, len);
	if(file->fops && file->fops->write)
		return file->fops->write(file, buffer, len);
	return 0; // Function not supported
}
/* TODO: add permission argument, ie: read-only, read-write, mount-update,... */
int VfsMount(char *device, char *mountpoint, char *fstype)
{
	MountStruct *mount;
	VnodeStruct *vnode;
	VnodeStruct *devnode;
	VnodeStruct *covered;

	DEBUG("mounting '%s' on '%s', fstype='%s'", mountpoint, device?device:"NULL", fstype); 
	mount = (MountStruct *)kmalloc(sizeof(MountStruct));
	if(mount == 0)
		 return 0;

	memset(mount, 0, sizeof(MountStruct));
	mount->fs = GetFileSystem(fstype);
	if(mount->fs == 0)
	{
		 kfree(mount);
		 DEBUG("No file system");
         return 0;
	}
	if(device != 0)
	{
		 // TODO: check that we are mounting a block device
		 devnode = Path2Vnode(device);
		 if(devnode == 0)
		 {
			  DEBUG("Device '%s' does not exist", device);
              return 0;		
		 }
		 mount->device = devnode->device;
	}	

	mount->mountpoint = kstrdup(mountpoint);
	if(mount->mountpoint == 0)
	{	
		 kfree(mount);
         return 0;
	}
	if(!RootVnode)
	{
		 // Nothing mounted yet
		 if(strcmp(mountpoint,"/") != 0)
		 {
			  // We need to mount root
              kfree(mount->mountpoint);
			  kfree(mount);
              return 0;
		 }
		 if(!mount->fs->fscalls || !mount->fs->fscalls->mount)
		 {
			  kfree(mount->mountpoint);
			  kfree(mount);
			  return 0;
		 }
		 mount->fs->fscalls->mount(mount, mount->device, (VnodeStruct **)&vnode); // Will call rootfs
		 mount->mount_cover = 0; // This is the root mount;
		 RootVnode = vnode;
	}
	else
	{
		 covered = Path2Vnode(mountpoint);
		 if(covered == 0)
		 {
			  kfree(mount->mountpoint);
			  kfree(mount);
			  return 0;
		 }	
		 if((covered != RootVnode) && (covered->mount->root_vnode == covered))
		 {
			  kfree(mount->mountpoint);
			  kfree(mount);
			  DEBUG("'%s' is already mounted", mountpoint);
              return 0;
		 }
		 // mount->mount_cover is the vnode of the filesystem that we are mounted
		 // to. So if we are mounting this to /usr/dir, the mount_cover will point
		 // to the vnode of /usr/dir (which is on a different mount).
		 mount->mount_cover = covered;
		 if(!mount->fs->fscalls || !mount->fs->fscalls->mount)
		 {
			  kfree(mount->mountpoint);
			  kfree(mount);
              return 0;
		 }
		 // call filesystem level mount
         if(!mount->fs->fscalls->mount(mount, mount->device, (VnodeStruct **)&vnode))
		 {
			  kfree(mount->mountpoint);
			  kfree(mount);
			  DEBUG("Could not mount device '%s' in fs '%s', at '%s'", device, fstype, mountpoint);
              return 0;
		 }
		 if(!vnode)
		 {
			  kfree(mount->mountpoint);
			  kfree(mount);
			  return 0;
		 }
	}
	mount->root_vnode = vnode;
	vnode->mount = mount;
	// The mount_cover vnode, is covered by the root vnode of this mount
	mount->mount_cover->covered_by = mount->root_vnode;
	// add vnode to mounts list;
	VnodeAddToMount(mount, vnode);
	AddMount(mount);
	return 1;
}
int VfsChangeDirectory(char *path)
{
	ProcessStruct *process;
	VnodeStruct *vnode;

	process = GetCurrentProcess();
	if(!process)
		 return -ENOPROC;

	vnode = Path2Vnode(path);
	if(!vnode)
		 return -ENOENT;

	if(!S_ISDIR(vnode->mode))
		 return -ENOTDIR;

	process->tcurrent->CurrentDir = vnode;
	return ESUCCESS;
}
int VfsFileStat(int fd, struct StatStruct *buf)
{
	ProcessStruct *process = GetCurrentProcess();
	int ret;

	if(process->file == 0)
		return -ENOTFILES;

	FileStruct *file = process->file[fd];
	if(file == 0)
		 return -EBADF;

	if(!file->vnode || !file->vnode->mount || 
		!file->vnode->mount->fs || 
		!file->vnode->mount->fs->fscalls || 
		!file->vnode->mount->fs->fscalls->stat)
    {
		 return -ENOTFOUND;
	}
	ret = file->vnode->mount->fs->fscalls->stat(file, buf);
	return ret;
}
int VfsStat(const char *path, struct StatStruct *buf)
{
	int fd = VfsOpen((char *)path, O_RDONLY);	
	if(fd < 0)
		 return -EIO;
	memset(buf, 0, sizeof(struct StatStruct));
	int ret = VfsFileStat(fd, buf);
	VfsClose(fd);
	return ret;
}
int VfsLongSeek(int fd, int offset, int from)
{
	ProcessStruct *process = GetCurrentProcess();
	FileStruct *file = process->file[fd];
	if(file == 0)
		 return 0;
	if(file->fops && file->fops->seek)
		 return file->fops->seek(file, offset, from);
	return 0;
}
int VfsMakeDirectory(char *path)
{
	VnodeStruct *vnode;
	char filename[MAX_FILENAME];
	int ret;

	DEBUG("path '%s'",path);

	vnode = VfsPathToDirVnode(path, filename);
	if(vnode == 0)
		 return 0;
	ret = vnode->mount->fs->fscalls->MakeDirectory(vnode, filename);
	return ret;
}
/** @brief  Gets the location of d_name in a struct dirent */
#define NAME_OFFSET(de) ((int) ((de)->d_name - (char *)(de)))
/** @brief Round Up */
#define ROUND_UP(x) (((x)+sizeof(long)-1) & ~(sizeof(long)-1))

/** @brief Get Directory Entries Callback struct */
struct getdentsCallback
{
	/** @brief Current directory */
    struct dirent *currentDir;
    /** @brief Previous directory */
	struct dirent *previous;
	/** @brief Count */
	int count;
    /** @brief Error */
	int error;
};

/** @fn static int VfsFilldir(void *__buf, const char *name, int namelen, long offset, unsigned short ino)
    @brief Fill Directory Function
    @ingroup sys_kernel_fs 
    @param __buf getdentsCallback structure pointer
    @param name Name
    @param namelen strlen(name)
    @param offset Offset
    @param ino Ino
    @return Error Code
    @internal
*/
static int VfsFilldir(void *__buf, const char *name, int namelen, long offset, unsigned short ino)
{
	struct dirent *dirent;
	struct getdentsCallback *callback = (struct getdentsCallback *)__buf;
	int reclen = ROUND_UP(NAME_OFFSET(dirent) + namelen + 1);

	callback->error = -1;
	if(reclen > callback->count)
		 return -1;
	dirent = callback->previous;
	if(dirent)
		 dirent->d_off = offset;
	dirent = callback->currentDir;
	callback->previous = dirent;
	
	dirent->d_ino = ino;
	dirent->d_reclen = reclen;
	
	memcpy(dirent->d_name, name, namelen);
	dirent->d_name[namelen] = 0;
	
	dirent = (struct dirent *)((char *)dirent + reclen);
	
	callback->currentDir = dirent;
	callback->count -= reclen;

	return 0;
}
int VfsGetDents(int fd, void *dirent, unsigned int count)
{
	struct getdentsCallback callback;
	struct dirent *lastdirent;
	int ret;

	ProcessStruct *process = GetCurrentProcess();
	FileStruct *file = process->file[fd];

	if(file == NULL)
		 return 0;
	if(!S_ISDIR(file->vnode->mode))
		 return 0;

	callback.currentDir = (struct dirent *)dirent;
	callback.previous = 0;
	callback.count = count;
	callback.error = 0;

	if(!file->vnode || !file->vnode->mount || !file->vnode->mount->fs ||
		!file->vnode->mount->fs->fscalls || 
		!file->vnode->mount->fs->fscalls->ReadDirectory)
	{
		 return 0;
	}
	ret = file->vnode->mount->fs->fscalls->ReadDirectory(file, &callback, VfsFilldir);
	if(ret < 0) // Read directory return true / false
		 return ret;
	ret = callback.error;
	lastdirent = callback.previous;
	if(lastdirent)
	{
		 lastdirent->d_off = file->pos;
		 ret = count - callback.count;
	}
	return ret; 
}
int VfsCreate(char *path, unsigned short mode)
{
	DEBUG("path:'%s', mode:0x%x", path, mode);
    VnodeStruct *vnode;
	char filename[MAX_FILENAME];
	
	vnode = VfsPathToDirVnode(path, filename);
	if(!vnode)
    {
         return -ENOENT;
	}
	DEBUG("vnode id = %d", vnode->id);
    if(!vnode->mount ||
		!vnode->mount->fs ||
		!vnode->mount->fs->fscalls ||
		!vnode->mount->fs->fscalls->create)
    {
		 DEBUG("Something in vnode is missing");
		 return -ENOTFOUND;
	}
	return vnode->mount->fs->fscalls->create(vnode, filename, mode);
}
int VfsRename(char *old, char *new)
{
	VnodeStruct *vnode;
	
	vnode = Path2Vnode(old);
	if(!vnode)
    {
		 return -EIO;
	}
	if(!vnode->mount ||
		!vnode->mount->fs ||
		!vnode->mount->fs->fscalls ||
		!vnode->mount->fs->fscalls->rename)
    {
		 return -ENOTFOUND;
	}
	return vnode->mount->fs->fscalls->rename(vnode, new);
}
int VfsRemove(char *path)
{
	VnodeStruct *vnode;
	
	vnode = Path2Vnode(path);
	if(!vnode)
    {
		 return -EIO;
	}
	if(!vnode->mount ||
		!vnode->mount->fs ||
		!vnode->mount->fs->fscalls ||
		!vnode->mount->fs->fscalls->remove)
    {
		 return -ENOTFOUND;
	}
	return vnode->mount->fs->fscalls->remove(vnode);
}
int VfsDuplicate(int oldfd)
{
	ProcessStruct *process;
	FileStruct *file;
	int i, ret;

	process = GetCurrentProcess();
	if(process == 0)
		 return -ENOPROC;
	if(oldfd < 0 || oldfd > MAX_FILE)
		 return -EBADF;

	file = process->file[oldfd];
	if(file == 0)
		 return -EBADF;

	ret = -EMFILE;
	for(i=0; i<MAX_FILE; i++)
    {
		 if(process->file[i] == 0)
         {
			  process->file[i] = file;
			  ret = i;
			  break;
		 }
	}
	return ret;
}
int VfsRegisterFs(char *name, FsCallsStruct *fscalls)
{
	FilesystemStruct *fs;
	FilesystemStruct *ptr;
	
	fs = FsListHead;
	
	if(fscalls == 0)
		return 0;
	if(FsListHead == 0)
	{
		 FsListHead = (FilesystemStruct *)kmalloc(sizeof(FilesystemStruct));
		 if(FsListHead == 0)
		 {
			  return 0;
		 }
		 memset(FsListHead, 0, sizeof(FilesystemStruct));
		 FsListHead->name = kstrdup(name);
		 if(FsListHead->name == 0)
		 {
			  kfree(FsListHead);
			  return 0;
		 }
		 FsListHead->fscalls = fscalls;
		 DEBUG("Registering initial FileSystem: %s", FsListHead->name);
		 return 1;
	}
	while(fs->next != 0)
	{
		 fs = fs->next;
	}
	// save a pointer to current filesystem in the list
	ptr = fs;
	fs->next = (FilesystemStruct *)kmalloc(sizeof(FilesystemStruct));
	if(fs->next == 0)
		return 0;

	fs = fs->next;
	memset(fs, 0, sizeof(FilesystemStruct));
	
	fs->name = kstrdup(name);
	if(fs->name == 0)
	{
		 kfree(fs);
		 ptr->next = 0;
		 return 0;
	}
	fs->fscalls = fscalls;
	DEBUG("Registering new FileSystem: %s", fs->name);
	return 1;
}
void VfsInit()
{
	FsListHead = 0;
	MountListHead = 0;
}
