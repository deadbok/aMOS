ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

all:
	$(MAKE) -C ./elf2mod
	$(MAKE) -C ./md5
	$(MAKE) -C ./elf

clean:
	$(MAKE) -C ./elf2mod clean
	$(MAKE) -C ./md5 clean
	$(MAKE) -C ./elf clean
	$(MAKE) -C ./lib clean


