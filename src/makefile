ROOTDIR = ..
include $(ROOTDIR)/Makefile.rules

all: boot loader libc kernel tools

.PHONY: boot
boot:
	$(MAKE) -C ./hik/boot

.PHONY: loader
loader:
	$(MAKE) -C ./hik/loader

.PHONY: libc
libc:
	$(MAKE) -C ./hik/libc

.PHONY: kernel
kernel:
	$(MAKE) -C ./hik/kernel

.PHONY: tools
tools:
	$(MAKE) -C ./tools

clean:
	$(RM) -f ./include/version.h
	$(MAKE) -C ./hik clean
	$(MAKE) -C ./tools clean


