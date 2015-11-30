ROOTDIR = ../..
include $(ROOTDIR)/Makefile.rules

OBJECTS = list.o
TARGET = ../../../lib/libutil.a

all: libutil

libutil:$(OBJECTS)
		$(AR) $(TARGET) $(OBJECTS)
		$(RANLIB) $(TARGET)

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst
