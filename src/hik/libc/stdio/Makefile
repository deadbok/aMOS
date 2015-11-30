ROOTDIR = ../../../..
include $(ROOTDIR)/Makefile.rules

CFLAGS = $(AMOS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_INCLUDE) $(CC_INCLUDE_FLAG)$(AMOS_HIK_INCLUDE)

OBJECTS = printf.o
TARGET = $(AMOS_HIK_LIB)/libc.a

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)


clean:
	$(RM) -f $(TARGET)
	$(RM) -f *.o
	$(RM) -f *.lst
