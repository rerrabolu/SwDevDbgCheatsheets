obj-m += re_module_4.o
re_module_4-objs := ram_module_4.o ram_fops_impl.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo cp re_module_4.ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	sudo depmod -a

