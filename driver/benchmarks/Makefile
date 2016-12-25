ifneq ($(KERNELRELEASE),)
obj-m += benchmark.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
endif

clean:
	make -C $(KDIR) M=$(PWD) clean
