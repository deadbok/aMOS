ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = ports.o int.o 8259.o halt.o cpu.o init_console.o gdt.o idt.o intsys.o
TARGET = ../$(ROOTDIR)/lib/libkernel.a

all: entry kernel

.PHONY: entry
entry: entry.o
		cp entry.o $(ROOTDIR)/kernel/entry.o

.PHONY:  int
int: int.S
		gasp -o int.S int.MS

kernel:$(OBJECTS)
		$(AR) $(TARGET) $(OBJECTS)
		$(RANLIB) $(TARGET)

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst
	rm -f entry.o
	rm -f entry.o.lst

