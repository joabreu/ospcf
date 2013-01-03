; -- PCF Operating System --
; See [docs\COPYRIGHT.txt] for more info
;
; file: system\kernel\arch\x86\idtasm.asm
; desc: Interrupt descriptor table aux functions

[GLOBAL _x86IdtFlush]

; Instals a new IDT
_x86IdtFlush:
    mov eax, [esp+4]  		; New IDT is passed as argument
    lidt [eax]        		; Loads new IDT
    ret
