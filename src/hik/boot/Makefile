ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

CFLAGS = $(AMOS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_INCLUDE) $(CC_INCLUDE_FLAG)$(AMOS_HIK_INCLUDE)

OBJECTS = boot.o
TARGET = boot.bin

all: $(ROOTDIR)/bin/boot.bin mkimage

$(ROOTDIR)/bin/boot.bin: $(OBJECTS)
	$(LD) -T boot.ld -Map boot.map $(OBJECTS) -o $(TARGET)
	$(OBJCOPY) $(TARGET) $(ROOTDIR)/bin/boot.bin

.PHONY: mkimage
mkimage:
	$(MAKE) -C ./tools


clean:
	$(MAKE) -C ./tools clean
	$(RM) $(TARGET)
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map
