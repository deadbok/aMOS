/*	boot.ld for aMOS */

OUTPUT_FORMAT("elf32-i386")
ENTRY(entry)
SECTIONS {
  .text 0x000000 : {
    *(.text)
    _etext = .;
    etext = _etext;
  }

  .data : {
    *(.data)
    _edata = .;
    edata = _edata;
  }

  .bss : {
    *(.bss)
    *(COMMON)
    _end = .;
    end = _end;
  }
}
