ifneq ($(KERNELRELEASE),)
obj-m += kprobe.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
endif

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f kprobe_user
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
kprobe_user:kprobe_user.c
	cc -g -Wall kprobe_user.c -o kprobe_user
