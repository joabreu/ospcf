; -- PCF Operating System --
; See [docs\COPYRIGHT.txt] for more info
;
; file: system\kernel\arch\x86\start32.asm
; desc: Kernel entry point after being loaded by GRUB

[GLOBAL _x86KernelEntry]	; Kernel entry point
[EXTERN _x86KernelStartup]     	; Kernel c begin

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary.
MBOOT_MEM_INFO      equ 1<<1	; Provide memory map
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; 'magic number' lets bootloader find the header
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

SECTION .text
ALIGN 4
MultiBootHeader:
	dd  MBOOT_HEADER_MAGIC
	dd  MBOOT_HEADER_FLAGS
	dd  MBOOT_CHECKSUM

STACKSIZE equ 0x4000 ; Constant 16Kb

_x86KernelEntry:
	mov esp, stack+STACKSIZE
	push esp	; Stack location
	push eax	; Multiboot magic number
	push ebx	; Multiboot info structure

	call _x86KernelStartup	; Pass control

	cli
	jmp $	; Infinite loop, should never be reached

SECTION .bss
ALIGN 4
stack:
	resb STACKSIZE	; Reserve 16K stack on a doubleword boundary
