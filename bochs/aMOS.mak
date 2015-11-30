ROOTDIR = ..
include $(ROOTDIR)/Makefile.rules

BOCHS_FLOPPY = "boot: a"  "floppya: 1_44=$(ROOTDIR)/bin/startup.bin, status=inserted"
BOCHS_LOGFILE = "log: bochs_simulation.txt"
BOCHS_SLOWIPS = "ips: 13000000"
BOCHS_NOCP = -nocontrolpanel

BOCHS_PARAM = $(BOCHS_NOCP) $(BOCHS_FLOPPY) $(BOCHS_LOGFILE)

.PHONY: all
all: floppy

.PHONY: floppy
floppy:
	bochs $(BOCHS_PARAM)

.PHONY: hd
hd:
	bochs "boot: c"  "diskc: file=$(ROOTDIR)/bin/startup.bin, cyl=306, heads=4, spt=17" "log: bochs_simulation.txt"

# "boot: a"  "floppya: 1_44=$(ROOTDIR)/../bin/startup.bin, status=inserted" 
