/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
 *
 * file: system\kernel.ld
 * desc: Kernel compilation rules
*/
/* System Kernel Entry function (first function of the system executed after GRUB performs the loading) */
ENTRY(_x86KernelEntry)
/* Sections of executable */
SECTIONS
{

    .text 0x100000 :
    {
         code = .; _code = .; __code = .;
         *(.text)
         . = ALIGN(4096);
    }

    .data :
    {
         data = .; _data = .; __data = .;
         *(.data)
         *(.rodata)
         . = ALIGN(4096);
    }

    .bss :
    {
         bss = .; _bss = .; __bss = .;
         *(.bss)
         . = ALIGN(4096);
    }

    end = .; _end = .; __end = .;
    /DISCARD/ : { *(.comment .note .eh_frame .dtors) }
}
