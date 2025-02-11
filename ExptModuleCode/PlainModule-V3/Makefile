
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := re_module_3.o
re_module_3-objs := ram_module_3.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo cp re_module_3.ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	sudo depmod -a

