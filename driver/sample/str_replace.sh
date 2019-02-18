#!/bin/bash
read -p "Current string " curr_str
read -p "Replace string " repl_str

echo "$curr_str is going to replace $repl_str"

sed -i 's/$curr_str/$repl_str/g' *.c Make* 
#sed -i 's/PACKET/MISC/g' *.c 
#sed -i 's/Packet/MISC/g' *.c
#rename 's/$curr_str/$repl_str/g' ./*
