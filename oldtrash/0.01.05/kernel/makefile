ROOTDIR = ..
include $(ROOTDIR)/Makefile.rules

OBJECTS = main.o kprintf.o coollook.o kmalloc.o mm.o
TARGET = $(ROOTDIR)/kernel.o
LIBS = -lkernel -lc -lcons -lutil

all: console kernel util link

kernel:	$(OBJECTS)

.PHONY: console
console:
	make -C console

.PHONY: util
util:
	make -C util


.PHONY: link
link:
	$(LD) -T $(ROOTDIR)/arch/i386/kernel.ld -Map kernel.map -o $(TARGET) entry.o $(OBJECTS) -L../$(ROOTDIR)/lib $(LIBS)


clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst
	rm -f $(TARGET)
	rm -f entry.o
	make -C console clean
