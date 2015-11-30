ROOTDIR = ../../..

include $(ROOTDIR)/Makefile.rules

CFLAGS = $(AMOS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_HIK_INCLUDE) $(CC_INCLUDE_FLAG)$(AMOS_INCLUDE)

OBJECTS = entry.o inthandl.o main.o mailbox.o task.o schedule.o mm.o list.o int.o \
video.o gdt.o idt.o pit.o pic.o tss.o debug.o halt.o ports.o dt.o cpu.o fd.o pmm.o

KERNEL_OBJ = kernel.o
BIN_TARGET = $(ROOTDIR)/bin/kernel.bin
LIBS = -lc

all: $(BIN_TARGET)

$(BIN_TARGET):	$(OBJECTS)
	$(LD) -T $(AMOS_HIK)/kernel.ld -Map kernel.map -o $(BIN_TARGET) $(OBJECTS) -L$(AMOS_HIK_LIB) $(LIBS)
	$(RM) $(TARGET)

clean:
	$(RM) -f $(TARGET)
	$(RM) -f *.o
	$(RM) -f *.lst
	$(RM) -f *.map

