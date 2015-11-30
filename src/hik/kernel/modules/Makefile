ROOTDIR = ../../../..

include $(ROOTDIR)/Makefile.rules

CFLAGS = $(AMOS_CFLAGS) $(CC_INCLUDE_FLAG)$(AMOS_HIK_INCLUDE) $(CC_INCLUDE_FLAG)$(AMOS_INCLUDE)

OBJECTS = vga.o

TARGETS = kernel.bin
LIBS = -lc

%.mod : o.%
	$(LD) -T module.ld -Map kernel.map -o $(KERNEL_OBJ) $(OBJECTS) -L$(AMOS_HIK_LIB) $(LIBS)

all: $(TARGET)

$(KERNEL_OBJ):	$(OBJECTS)
	$(LD) -T $(AMOS_HIK)/kernel.ld -Map kernel.map -o $(KERNEL_OBJ) $(OBJECTS) -L$(AMOS_HIK_LIB) $(LIBS)
	rm -f $(TARGET)
		
$(TARGET):	$(KERNEL_OBJ)
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(KERNEL_OBJ) $(TARGET)
	$(CP) kernel.bin $(ROOTDIR)/bin

clean:
	$(RM) $(TARGET)
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map
	$(RM) *.bin
		

