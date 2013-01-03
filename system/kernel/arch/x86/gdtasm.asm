; -- PCF Operating System --
; See [docs\COPYRIGHT.txt] for more info
;
; file: system\kernel\arch\x86\gdtasm.asm
; desc: Global descriptor table aux functions

[GLOBAL _x86GdtFlush]
; Instals a new GDT
_x86GdtFlush:
    mov eax, [esp+4]  ; GDT location passed as an argument
    lgdt [eax]        ; Loads new GDT
;
; "RESET" CPU to use new GDT
;
    mov ax, 0x10      ; Data segment is 0x10 -> Each segment = 8 bytes
		      ; and data segment is 3rd, so 0x08+0x08 = 0x10
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
		      ; Code segment needs a far jump to be loaded
.flush:
    ret		      ; return
