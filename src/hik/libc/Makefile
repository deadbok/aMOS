ROOTDIR = ../../..
include $(ROOTDIR)/Makefile.rules

all: stdio string

.PHONY: stdio
stdio:
	$(MAKE) -C ./stdio

.PHONY: string
string:
	$(MAKE) -C ./string

clean:
	$(MAKE) -C ./stdio clean
	$(MAKE) -C ./string clean


