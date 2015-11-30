ROOTDIR = ../aMOS/src
include $(ROOTDIR)/Makefile.rules

.PHONY: all
all: floppy

.PHONY: floppy
floppy:
	./bochs.debug "boot: a"  "floppya: 1_44=$(ROOTDIR)/../bin/startup.bin, status=inserted" "log: $(ROOTDIR)/../bochs_simulation.log"
		
.PHONY: hd
hd:
	./bochs.debug "boot: c"  "diskc: file=$(ROOTDIR)/../bin/startup.bin, cyl=306, heads=4, spt=17" "log: $(ROOTDIR)/../bochs_simulation.log"

# "boot: a"  "floppya: 1_44=$(ROOTDIR)/../bin/startup.bin, status=inserted" 
