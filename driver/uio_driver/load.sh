#/bin/sh!!
sudo make
sudo modprobe uio
sudo insmod kpart.ko
sudo make upart
sudo ./upart
