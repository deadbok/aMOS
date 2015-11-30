ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

all: boot loader kernel mkimage_config

.PHONY: kernel
kernel:
	$(MAKE) -C ./kernel

.PHONY: boot
boot:
	$(MAKE) -C ./boot

.PHONY: loader
loader:
	$(MAKE) -C ./loader

.PHONY: mkimage_config	
	$(CP) boot.cfg $(ROOTDIR)/bin/

clean:
	make -C ./boot clean
	make -C ./loader clean
	make -C ./kernel clean
	make -C ./libc clean

