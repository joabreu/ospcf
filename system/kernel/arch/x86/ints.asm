; -- PCF Operating System --
; See [docs\COPYRIGHT.txt] for more info
;
; file: system\kernel\arch\x86\ints.asm
; desc: Interrupt handling aux functions

; ISR functions, no error code
; ISR number is passed as argument
%macro ISR_NO_ERR 1
    [GLOBAL _x86Isr%1AsmHandler]
    _x86Isr%1AsmHandler:
         cli			; Disables interruptions
    	 push byte 0		; Error code = 0
    	 push byte %1		; ISR number is argument
    	 jmp x86IsrAsmHandler	; Jumps to ISR handler
%endmacro

; ISR functions, error code
; ISR number is passed as argument
%macro ISR_ERR 1
    [GLOBAL _x86Isr%1AsmHandler]
    _x86Isr%1AsmHandler:
         cli			; Disables interruptions
    	 push byte %1		; ISR number as argument
	 ; Error code is already in stack
    	 jmp x86IsrAsmHandler	; Jumps to ISR handler
%endmacro

; All ISR			; X: Name ( Error Code )
ISR_NO_ERR	0		; 0: Divide Error Exception
ISR_NO_ERR	1		; 1: Debug Exception
ISR_NO_ERR	2		; 2: NonMaskable Interrupt "NMI"
ISR_NO_ERR	3		; 3: Breakpoint Exception
ISR_NO_ERR	4		; 4: Overflow Exception in INTO
ISR_NO_ERR	5		; 5: Out Of Bounds Exception
ISR_NO_ERR	6		; 6: Invalid Opcode Exception
ISR_NO_ERR	7		; 7: Device Not Available Exception
ISR_ERR		8		; 8: Double Fault Exception ( Sempre 0 )
ISR_NO_ERR	9		; 9: Coprocessor Segment Overrun
; ^ Intel Reserved; Do NOT USE. Recent IA-32 processors do not generate this exception
ISR_ERR		10		; 10: Invalid TSS Exception ( Segment Selector Index )
ISR_ERR		11		; 11: Segment Not Present ( Segment Selector Index )
ISR_ERR		12		; 12: Stack Fault Exception ( Segment Selector or 0 )
ISR_ERR		13		; 13: General Protection Exception ( IDT Segment Selector
; or IDT vector number or 0 )
ISR_ERR		14		; 14: Page Fault Exception ( Special Error Code [see :
; "Intel 64 and IA-32 Architectures Software Developer's Manual, Volume 3A - System
; Programming Guide" pg. 238. ])
ISR_NO_ERR	15		; 15: Unknown , This is not on Intel Manuals :O
ISR_NO_ERR	16		; 16: X87 FPU Floating Point Error
ISR_ERR		17		; 17: Alignment Check Exception ( Sempre 0 )
ISR_NO_ERR	18		; 18: Machine-Check Exception
ISR_NO_ERR	19		; 19: SIMD Floating-Point Exception
ISR_NO_ERR	20		; 20: Reserved
ISR_NO_ERR	21		; 21: Reserved
ISR_NO_ERR	22		; 22: Reserved
ISR_NO_ERR	23		; 23: Reserved
ISR_NO_ERR	24		; 24: Reserved
ISR_NO_ERR	25		; 25: Reserved
ISR_NO_ERR	26		; 26: Reserved
ISR_NO_ERR	27		; 27: Reserved
ISR_NO_ERR	28		; 28: Reserved
ISR_NO_ERR	29		; 29: Reserved
ISR_NO_ERR	30		; 30: Reserved
ISR_NO_ERR	31		; 31: Reserved

; 32... 255 - User Defined Interrupts

[EXTERN _x86IsrFaultHandler]	; isr.c

; ISR handler. Saves CPU current status and jumps to C handler (isr.c)
; Upon return sets previous CPU state
x86IsrAsmHandler:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, _x86IsrFaultHandler
    call eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret    

; IRQ functions
; IRQ number and IDT int number are passed as arguments
%macro IRQ 2
    [GLOBAL _x86Irq%1AsmHandler]
    _x86Irq%1AsmHandler:
         cli			; Disables interruptions
    	 push byte 0		; Dummy, error code
    	 push byte %2		; IRQ number (NO IDT)
    	 jmp x86IrqAsmHandler
%endmacro

; 0:(32) Timer Interrupt (PIT)
[EXTERN _x86ThreadArchSchedule]
[GLOBAL _x86Irq0AsmHandler]			; 0: Timer
_x86Irq0AsmHandler:
    cli

    pusha           ; push general registers onto stack
    push ds         ; push ds onto stack
    push es         ; push es 
    push fs         ; push fs
    push gs         ; push gs

    mov eax, 0x10   ; save selector
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    mov eax, esp    ; stack => eax
    push eax        ; push eax (esp)
	
    call _x86ThreadArchSchedule  ; sched(eax) returns stack of next task
    mov esp, eax    ; return value => stack

    mov al, 0x20    ; ack IRQ
    out 0x20, al

    pop gs          ; pop gs off task stack
    pop fs          ; pop fs
    pop es          ; pop es
    pop ds          ; pop ds
    popa            ; pop general registers
	
    push byte 0
    push byte 32
    jmp x86IrqAsmHandler

IRQ   1,    33			; 1: Keyboard Interrupt
IRQ   2,    34			; 2: Cascade for 8259A Slave controller
IRQ   3,    35			; 3: Serial port 2
IRQ   4,    36			; 4: Serial port 1
IRQ   5,    37			; 5: AT systems: Parallel Port 2. PS/2 systems: reserved
IRQ   6,    38			; 6: Floppy Interrupt
IRQ   7,    39			; 7: Parallel Port 1
IRQ   8,    40			; 8: Real Time Clock (CMOS)
IRQ   9,    41			; 9: CGA vertical retrace
IRQ  10,    42			; 10: Reserved
IRQ  11,    43			; 11: Reserved
IRQ  12,    44			; 12: AT systems: reserved. PS/2: auxiliary device
IRQ  13,    45			; 13: FPU
IRQ  14,    46			; 14: Hard disk controller
IRQ  15,    47			; 15: Reserved

; 16: Syscall Interrupt
[EXTERN _SyscallHandler]
[GLOBAL _x86SyscallAsmHandler]
_x86SyscallAsmHandler:
    cli			; Disables interruptions
    push byte 0		; Dummy, error code
    push 128		; IRQ number (NO IDT)
    
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, _SyscallHandler
    call eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

; 17:(49) re-Schedule Interrupt
[GLOBAL _x86Irq17AsmHandler]
_x86Irq17AsmHandler:
    cli

    pusha           ; push general registers onto stack
    push ds         ; push ds onto stack
    push es         ; push es 
    push fs         ; push fs
    push gs         ; push gs

    mov eax, 0x10   ; save selector
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    mov eax, esp    ; stack => eax
    push eax        ; push eax (esp)
	
    call _x86ThreadArchSchedule  ; sched(eax) returns stack of next task
    mov esp, eax    ; return value => stack

    mov al, 0x20    ; ack IRQ
    out 0x20, al

    pop gs          ; pop gs off task stack
    pop fs          ; pop fs
    pop es          ; pop es
    pop ds          ; pop ds
    popa            ; pop general registers
	
    iret


[EXTERN _x86IrqHandler]		; irq.c
; IRQ handler. Saves CPU current state and jumps to C handler (irq.c)
; Upon return restores CPU previous state
x86IrqAsmHandler:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, _x86IrqHandler
    call eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
