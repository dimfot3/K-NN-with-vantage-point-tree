#!/bin/bash

dist=2

for n in {10..60000..5000}
do
	for i in {1..5}
	do
    	./main_cpu_program ../data/dt_${dist}_${n}_2000.dat 0
   	done
done
