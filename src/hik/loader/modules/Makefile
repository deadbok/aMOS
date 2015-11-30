ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = video.o
TARGET = video.module


all: loader.o binize

loader:	$(OBJECTS)

$(TARGET):$(OBJECTS)
	$(LD) -T loader.ld -Map loader.map $(OBJECTS) -o $(TARGET)

.PHONY: binize
binize: 
		$(OBJCOPY) $(TARGET) loader.bin
		cp loader.bin ../$(ROOTDIR)/bin

clean:
	$(RM) $(OBJECTS)
	$(RM) $(addsuffix .lst,$(basename $(OBJECTS)))
	$(RM) $(TARGET)
	$(RM) *.bin
	$(RM) *.map