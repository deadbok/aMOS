ROOTDIR = ../../../..
include $(ROOTDIR)/Makefile.rules

CFLAGS = $(AMOS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_INCLUDE) $(CC_INCLUDE_FLAG)$(AMOS_HIK_INCLUDE)

OBJECTS = strcmp.o strlen.o strcpy.o memset.o memcpy.o memcmp.o malloc.o free.o
TARGET = $(AMOS_HIK_LIB)/libc.a

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.o
	$(RM) *.lst
