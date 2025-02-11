# Makefile for building simple kernel module

# Define the kernel directory for building
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# The object files for the module
obj-m := re_module_1.o
re_module_1-objs := ram_module_1.o

# Default target to build the module
all:
	make -C $(KDIR) M=$(PWD) modules

# Install target: copy the .ko file and run depmod
install:
	sudo cp re_module_1.ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	sudo depmod -a

# Clean target: clean the build files
clean:
	make -C $(KDIR) M=$(PWD) clean

