ROOTDIR = ../../../..
CFLAGS = -O3 -Wall -I$(ROOTDIR)/include/boot
CC = gcc

OBJECTS = mkimage.o

.c.o:
	$(CC) $(CFLAGS) -o $@ $<


all: mkimage.exe
	cp $(OBJECTS) ../$(ROOTDIR)/bin/mkimage

mkimage.exe: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJECTS).lst

