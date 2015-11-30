ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = strcmp.o strlen.o memset.o memcpy.o
TARGET = $(ROOTDIR)/../lib/libc.a

all: string
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)


string:$(OBJECTS)

clean:
	rm -f $(OBJECTS)

