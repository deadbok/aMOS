ROOTDIR = ..
include $(ROOTDIR)/Makefile.rules

all: stdio string

.PHONY: stdio
stdio:
	make -C stdio

.PHONY: string
string:
	make -C string

clean:
	make -C stdio clean
	make -C string clean


