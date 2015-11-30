ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

CFLAGS = $(AMOS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_INCLUDE)$(AMOS_HIK_INCLUDE)
OBJECTS = stub.o main.o video.o
TARGET = loader.bin


all: loader.bin

loader:	$(OBJECTS)

$(TARGET):$(OBJECTS)
	$(LD) $(LD_STRIP) -T loader.ld -Map loader.map $(OBJECTS) -o $(TARGET)
	$(CP) loader.bin $(ROOTDIR)/bin

clean:
	$(RM) $(TARGET)
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map
