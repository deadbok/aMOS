ROOTDIR = .
include $(ROOTDIR)/Makefile.rules

all: version.h boot loader libc kernel binize

version.h:
	@echo \#ifndef _KERNEL_VERSION_H_ > $(ROOTDIR)/include/kernel/version.h
	@echo \#define _KERNEL_VERSION_H_ >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define STRING_VERSION \"$(VERSION)\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define BUILD_ARCH \"$(CPU)\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define COMPILE_DATE \"`date +%m/%d/%Y`\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define COMPILE_TIME \"`date +%T`\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define COMPILE_BY \"`whoami`\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define COMPILE_HOST \"`hostname`\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#define COMPILER \"`$(CC) $(CFLAGS) -v 2>&1 | tail -1`\" >> $(ROOTDIR)/include/kernel/version.h
	@echo \#endif >> $(ROOTDIR)/include/kernel/version.h
	@echo version.h updated...

.PHONY: boot
boot:
	make -C arch/i386/boot

.PHONY: loader
loader:
	make -C arch/i386/loader

.PHONY: libc
libc:
	make -C libc

.PHONY: kernel
kernel:
	make -C arch/i386/kernel
	make -C kernel

.PHONY: binize
binize:
	$(OBJCOPY) kernel.o kernel.bin
	cp kernel.bin ../bin
	rm -f kernel/coollook.o

clean:
	rm -f $(ROOTDIR)/include/kernel/version.h
	rm -f kernel.o
	make -C arch/i386 clean
	make -C kernel clean
	make -C libc clean

