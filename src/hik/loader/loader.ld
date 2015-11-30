/*	loader.ld for aMOS */

OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS {
	. = 0x8000;
	.text . : {
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
