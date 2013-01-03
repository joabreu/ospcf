/* -- PCF Operating System --
 * See [docs\COPYRIGHT.txt] for more info
*/
/** @file include\system\kernel\x86\grub.h
    @brief GRUB multiboot
*/
#ifndef INCLUDE_SYSTEM_KERNEL_X86_GRUB_H
#define INCLUDE_SYSTEM_KERNEL_X86_GRUB_H

/** @brief Elf Struct To GRUB */
struct ElfSectionHeaderTableStruct
{
    /** @brief Number */
    unsigned long Num;
    /** @brief Size */
    unsigned long Size;
    /** @brief Address */
    unsigned long Addr;
    /** @brief Section Header index */
    unsigned long SectionHeaderIndex;
};
/** @brief Memory map of system by GRUB */
struct MultibootMemoryMapStruct
{
	/** @brief Memory size */
	unsigned long int Size;
	/** @brief Base address low */
	unsigned long int BaseAddrLow;
	/** @brief Base address high */
	unsigned long int BaseAddrHigh;
	/** @brief Length low */
	unsigned long int LengthLow;
	/** @brief Length high */
	unsigned long int LengthHigh;
	/** @brief Type */
	unsigned long int Type;
};
/** @brief The standard struct of a GRUB module */
struct MultibootModuleStruct
{
	/** @brief Module start address */
	unsigned long ModuleStart;
	/** @brief Module end address */
	unsigned long ModuleEnd;
	/** @brief String */
	unsigned long String;
	/** @brief Reserved */
	unsigned long Reserved;
};
/** @brief System info by GRUB */
typedef struct
{
    /** @brief Flags */
	unsigned long int Flags;
    /** @brief Size of low memory (below 1 MB) (Kbytes) */
	unsigned long int MemoryLower;
    /** @brief Size of memory above 1 MB (Kbytes) */
	unsigned long int MemoryUpper;
    /** @brief Boot device BIOS string */
	unsigned char  BootDevice[4];
	/** @brief Commands passed to kernel */
    unsigned long int CmdLine;
    /** @brief Modules count */
	unsigned long int ModulesCount;
    /** @brief Modules address */
	unsigned long int ModulesAddr;
    /** @brief Elf Section Header */
	struct ElfSectionHeaderTableStruct ElfSec;
    /** @brief Memory map length */
	unsigned long int MemoryMapLength;
    /** @brief Memory Map */
	struct MultibootMemoryMapStruct  *MemoryMap;
    
    /** @brief Drives length */
	unsigned long int DrivesLength;
    /** @brief Drives address */
	unsigned long int DrivesAddr;
   	/** @brief Configuration table */
	unsigned long int ConfigTable;
    /** @brief Boot loader name */
	unsigned long int BootLoaderName;
    
    /** @brief APM table */
	unsigned long int ApmTable;
    /** @brief VBE control info */
	unsigned long int VbeControlInfo;
    /** @brief VBE mode info */
	unsigned long int VbeModeInfo;
    /** @brief VBE mode */
	unsigned short int  VbeMode;
    /** @brief VBE interface segment */
	unsigned short int  VbeInterfaceSeg;
    /** @brief VBE interface offset */
	unsigned short int  VbeInterfaceOff;
    /** @brief VBE interface length */
	unsigned short int  VbeInterfaceLen;
} MultibootInfoStruct;

#endif
