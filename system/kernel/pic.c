/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file system\kernel\pic.c
    @brief PIC functions
*/
#include <kernel\pic.h>
#include <kernel\x86\io.h>

/** @brief Information about IRQ's base and mask for PIC functions */
static struct
{
	unsigned char  base;
	unsigned short mask;
} Irqs;

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
void PicRemap(unsigned char start)
{
    Irqs.base = start & 0xf8;
	Irqs.mask = 0xffff;
	
    // ICW 1 - Inicializa PIC's  
    outportb(MASTER_PIC, 0x11);    // Master
    outportb(SLAVE_PIC, 0x11);    // Slave
    // ICW 2 - Remapeia PIC's
    outportb(MASTER_PIC + 1, Irqs.base);   // Master
    outportb(SLAVE_PIC + 1, Irqs.base+8); // Slave
    // ICW 3 - Dá a conhecer aos PIC's as portas de comunicação
    outportb(MASTER_PIC + 1, 0x04);    // Master write to IRQ2 of Slave
    outportb(SLAVE_PIC + 1, 0x02);    // Slave write to IRQ2 of Master
    // ICW 4 - Enables 80x86 Mode
    outportb(MASTER_PIC + 1, 0x01);
    outportb(SLAVE_PIC + 1, 0x01);
    // All done reseta os registos
    outportb(MASTER_PIC + 1, 0x0);
    outportb(SLAVE_PIC + 1, 0x0);
}
void PicEnableIrq(int num)
{
	Irqs.mask &= ~(1 << num);
	if(num < 8)
		 outportb(MASTER_PIC + 1, Irqs.mask);
	else
	{
		 if(Irqs.mask & 4)
              PicEnableIrq(2);
		 outportb(SLAVE_PIC + 1, Irqs.mask >> 8);
	}
}
void PicDisableIrq(int num)
{
	Irqs.mask |= (1 << num);
	if(num < 8)
		 outportb(MASTER_PIC + 1, Irqs.mask);
	else
	{
		 if((Irqs.mask >> 8) == 0xff && !(Irqs.mask & 4))
              PicDisableIrq(2);
		 outportb(SLAVE_PIC + 1, Irqs.mask >> 8);
	}
}
