/*	kernel.ld for aMOS */

/* 	This is temporay as the aMOS object format will be defined later on.
 *	When this happens there's a lot of clean up to do, first of all the
 *	kernel shuold be link to produce an ordinary aMOS-executeable.
 *	The loader should support this format as well as multiboot, ELF,
 *	and COFF. When the kernel get this far it will be possible to
 *	get the following values from the kernel header
 */

OUTPUT_FORMAT("elf32-i386")
ENTRY(k32_startup)
SECTIONS {
  .text 0x100000 : {
    *(.text)
    _etext = .;
    etext = _etext;
  }

  .data : {
    *(.data)
    _edata = .;
    edata = _edata;
  }

  . = ALIGN(4096);
  .data.aligned : { *(.data.idt) }

  .bss : {
    *(.bss)
    *(COMMON)
    _end = .;
    end = _end;
  }
}
