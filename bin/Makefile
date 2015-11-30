ROOTDIR = ..
include $(ROOTDIR)/Makefile.rules

.PHONY: all
all: bochs

.PHONY: all_debug
all_debug: bochs_debug

.PHONY: floppy
floppy: trans_floppy

		
.PHONY: bochs
bochs: bochs_floppy

.PHONY: bochs_debug
bochs_debug:
	./mkimage 1474560
	make -C $(ROOTDIR)/bochs -f aMOSdebug.mak floppy

.PHONY: bochs_floppy
bochs_floppy:
	./mkimage -p 1474560
	make -C $(ROOTDIR)/bochs -f aMOS.mak floppy
		
.PHONY: bochs_hd
bochs_hd:
	./mkimage -p
	make -C $(ROOTDIR)/bochs -f aMOS.mak hd
		
.PHONY: trans_floppy
trans_floppy:
	./mkimage
	dd if=startup.bin bs=512 of=/dev/fd0

.PHONY: clean		
clean:
	$(RM) -f *.bin
	$(RM) -f mkimage

