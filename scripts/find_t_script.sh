#!/bin/bash


for n in {1..50..1}
do
	for i in {1..20}
	do
    	./main_cpu_program ../data/dt_2_55010_2000.dat 2 $n
   	done
done
