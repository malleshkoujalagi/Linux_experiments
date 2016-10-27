#!/bin/bash
#gdb version:	-net none -no-hpet -kernel ./linux/arch/x86/boot/bzImage -s -S \
./qemu/build/x86_64-softmmu/qemu-system-x86_64 -cpu host -M pc -enable-kvm -name vm1 \
	-pidfile /tmp/vm1.pid -smp 1,threads=1,sockets=1 -m 2GB -mem-prealloc \
	-drive if=virtio,file=./rootfs/vm1.img,format=raw \
	-net nic,macaddr=52:54:00:fa:ce:01,model=virtio -net user,hostfwd=tcp::2001-:22 \
	-no-hpet -kernel ./linux/arch/x86/boot/bzImage -s -S \
	-append 'root=/dev/vda earlyprintk crashkernel=auto net.ifname=0 \
	 biosdevname=0 console=ttyS0,115200,8n1 3 rw transparent_hugepages=never \
	 noplymouth nohz_full=0 rcu_nocbs=0' -nographic -monitor none
