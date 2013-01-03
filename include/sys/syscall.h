/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\sys\syscall.h
    @brief System calls values
*/
#ifndef INCLUDE_SYS_SYSCALL_H
#define INCLUDE_SYS_SYSCALL_H

// Available system calls
enum {
	SYSTEMCALL_NOCALL = 0,
	
	// Process system calls
    SYSTEMCALL_THREAD_EXIT,
    SYSTEMCALL_READ,
    SYSTEMCALL_WRITE,
    SYSTEMCALL_TEST,

   	SYSTEMCALL_MAXINDEX,
};

/** @brief Interface to call a system call
    @param nr System call number, see enum above
    @param p1 Pointer to first argument
    @param ret The syscall return is stored here */
#define SYSTEM_CALL_CORE(nr,p1,ret )\
	__asm__ volatile("int $0x80" :: "a"(nr), "b"(p1), "c"(ret));

#endif
