obj-m += re_module_9.o
re_module_9-objs := ram_module_9.o ram_fops.o ram_fops_struct.o
re_module_9-objs += ram_fops_core.o ram_fops_impl.o ram_dev_ioctl_impl.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo cp re_module_9.ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	sudo depmod -a

