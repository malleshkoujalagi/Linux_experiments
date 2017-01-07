#!/bin/bash
for gov in $(ls /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor)
do
  echo "performance" > $gov
done
