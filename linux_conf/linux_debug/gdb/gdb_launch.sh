#!/bin/bash
gdb -ex 'target remote localhost:1234' -ex 'set breakpoint auto-hw' -ex 'break start_kernel' -ex c linux/vmlinux
