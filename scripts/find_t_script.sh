#!/bin/bash

for data in dt_2_800_500.dat dt_2_10000_2000.dat dt_2_20000_20000.dat dt_2_30000_5000.dat dt_2_50000_20000.dat
do
	for i in {1..10}
	do
		mpirun -np 2 main_cpu_program ../data/$data 3 4
   	done
done
