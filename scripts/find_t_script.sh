#!/bin/bash

for data in dt_2_10000_3000.dat dt_2_30000_10000.dat dt_2_55000_3000.dat
do
	for n in {1..15..1}
	do
		for i in {1..10}
		do
			./main_cpu_program ../data/$data $1 $n
	   	done
	done
done
