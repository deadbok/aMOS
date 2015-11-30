ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = console.o
TARGET = ../../../lib/libcons.a

all: libcons

libcons:$(OBJECTS)
		$(AR) $(TARGET) $(OBJECTS)
		$(RANLIB) $(TARGET)

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst
