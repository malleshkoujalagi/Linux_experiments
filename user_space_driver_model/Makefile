ifneq ($(KERNELRELEASE),)
obj-m += kpart.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
endif

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f upart
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
upart:upart.c
	cc -g -Wall upart.c -o upart
