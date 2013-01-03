/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
 *
 * file: system\kernel\arch\x86\tasks.s
 * desc: Tasks management, low level functions
*/
.global _x86ThreadExitEntry

ReturnValue:
.long 0

/* Exiting from a thread */
_x86ThreadExitEntry:
	pushl	%eax			/* Return value of thread is on eax, add it to the stack */
	mov	%esp, %ebx		/* Passes esp as an argument (return value is on the top) */
	movl	$1, %eax		/* System call number SYSTEMCALL_THREAD_EXIT = 1 */
	movl	$ReturnValue, %ecx	/* Return pointer, WARNING: This var may not be */
					/* 		   addresed by user tasks */

	int	$0x80			/* Call interrupt */
	
	jmp _x86ThreadExitEntry
