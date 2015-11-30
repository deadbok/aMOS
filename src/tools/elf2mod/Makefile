ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

CFLAGS = $(HOST_OS_CFLAGS) $(CC_INCLUDE_FLAG)
TARGET = elf2mod
BIN_TARGET = $(ROOTDIR)/bin/$(TARGET)

all: $(BIN_TARGET)

$(BIN_TARGET): $(TARGET)
	$(CP) $(TARGET) $(BIN_TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map
