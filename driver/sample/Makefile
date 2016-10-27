ifneq ($(KERNELRELEASE),)
obj-m += packet.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
endif

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f packet_userspace
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
packet_userspace:packet_userspace.c
	cc -g -Wall packet_userspace.c -o packet_userspace
