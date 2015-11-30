#The /bin makefiles are not complete. The user should be able to "make clean" from this directory, to
#clean every thing up, just like "make clean" in the root directory does. This requeres, that a variable
#called LEVEL should store the level in the directory tree at wich the makefile was started. Then the rest
#would be a matter of checking whether we were called from root level or start in /bin.

#26072001 22:00: added targets to make doxygen documentation, and rearanged a bit in the process :-)

ROOTDIR = .

include $(ROOTDIR)/Makefile.rules

.PHONY: init
init:
	$(ECHO) "\n\n-*- Compiling on a $(HOST_OS) system -*-\n"
	$(MAKE) $(HOST_OS_RULES) version.h

.PHONY: all
all: init docs aMOS bochs_simulation


.PHONY: aMOS
aMOS: init compile

.PHONY: compile
compile:
	$(MAKE) -C ./src

.PHONY: docs
docs: init doxy-docs

.PHONY: doxy-docs
doxy-docs:
	$(DOXYGEN) aMOS.Doxyfile

.PHONY: bochs_simulation
bochs_simulation:
	$(MAKE) -C ./bin


.PHONY: src_dist
src_dist: clean
	$(RM) aMOS-$(VERSION).tar.gz
	tar -cvzf aMOS-$(VERSION).tar.gz ./src/* ./$(HOST_OS)/*

.PHONY: all_dist
all_dist: clean
	$(RM) aMOS-$(VERSION).tar.gz
	tar -cvzf aMOS-$(VERSION).tar.gz ./*

.PHONY: cvs-update
cvs-update: clean
	cvs update -I*.bak -I*.lst -I*.o

.PHONY: clean
clean:
	$(MAKE) -C ./bin clean
	$(MAKE) -C ./src clean
	$(MAKE) -C ./docs clean
	$(ECHO) "\n"
