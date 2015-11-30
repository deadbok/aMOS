ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = printf.o
TARGET = $(ROOTDIR)/../lib/libc.a

all: stdio
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)


stdio:$(OBJECTS)

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst
