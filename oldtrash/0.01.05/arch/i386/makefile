ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

all: boot loader kernel

.PHONY: kernel
kernel:
	make -C kernel

.PHONY: boot
boot:
	make -C boot

.PHONY: loader
loader:
	make -C loader

clean:
	make -C boot clean
	make -C loader clean
	make -C kernel clean

