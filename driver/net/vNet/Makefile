ifneq ($(KERNELRELEASE),)
obj-m += vNet.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
endif

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f vNet_usr
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
vNet_usr:vNet_usr.c
	cc -g -Wall vNet_usr.c -o vNet_usr
