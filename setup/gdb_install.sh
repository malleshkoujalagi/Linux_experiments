#!/bin/bash
wget http://ftp.gnu.org/gnu/gdb/gdb-7.11.tar.xz
tar -xf gdb-7.8.tar.xz
cd gdb-7.8/
./configure
make
sudo cp gdb/gdb /usr/local/bin/gdb
