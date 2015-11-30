ROOTDIR = ../../../..
include $(ROOTDIR)/Makefile.rules

CFLAGS = $(HOST_OS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_HIK_BOOT_INCLUDE)
TARGET = mkimage
EXE_TARGET = $(ROOTDIR)/bin/$(TARGET)

all: $(EXE_TARGET)

$(EXE_TARGET): $(TARGET)
	$(CP) $(TARGET) $(ROOTDIR)/bin/$(TARGET)

$(TARGET):

clean:
	$(RM) $(TARGET)
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map
