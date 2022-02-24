#!/bin/bash

##mode 3
for data in dt_2_800_500.dat dt_2_10000_2000.dat dt_2_20000_20000.dat dt_2_30000_5000.dat dt_2_50000_20000.dat
do
	for i in {1..5}
	do
		mpirun -np 2 main_cpu_program ../data/$data 3 4
   	done
done

##mode 0,1,2
for data in dt_2_800_500.dat dt_2_10000_2000.dat dt_2_20000_20000.dat dt_2_30000_5000.dat dt_2_50000_20000.dat
do
	for j in {0..3}
	do
		for i in {1..5}
		do
			./main_cpu_program ../data/$data $j 4
	   	done
   	done
done
