ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = stub.o main.o
TARGET = loader.o


all: loader.o binize

loader:	$(OBJECTS)

$(TARGET):$(OBJECTS)
	$(LD) -T loader.ld -Map loader.map $(OBJECTS) -o $(TARGET)

.PHONY: binize
binize: 
		$(OBJCOPY) $(TARGET) loader.bin
		cp loader.bin ../$(ROOTDIR)/bin

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst
	rm -f $(TARGET)
	rm -f loader.bin