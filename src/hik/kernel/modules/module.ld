OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(module_info)
SECTIONS {
  .text 0x00000000 : {
    *(.text)
    _etext = .;
    etext = _etext;
  }

  .data : {
    *(.data)
    *(.rodata)
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
