ROOTDIR = ../../../
include $(ROOTDIR)/Makefile.rules

OBJECTS = boot.o

all: boot mkimage
	cp boot.o ../$(ROOTDIR)/bin/boot.bin

boot:
	$(NASM) -l boot.o.lst -o boot.o boot.asm

.PHONY: mkimage
mkimage:
	make -C tools


clean:
	make -C tools clean
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst