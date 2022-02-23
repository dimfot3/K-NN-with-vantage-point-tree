#!/bin/bash

for n in {1..50..1}
do
	for i in {1..20}
	do
    	./main_cpu_program ../data/dt_2_30000_10000.dat $1 $n
   	done
done
