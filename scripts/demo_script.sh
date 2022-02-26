#!/bin/bash

# configure and build the project
cd ..
mkdir -p build && cd build
cmake ..
make 

## ./main_cpu_program <data path> <mode> <number of live threads> <KNN search bool>

#run the mode 0(sequential) (KNN task is on) (number of live threads has no effect)
./main_cpu_program ../data/dt_2_1200_800.dat 0 0 1

#1(simple OpenMP) (number of live threads has no effect) (KNN task is off)
./main_cpu_program ../data/dt_2_10000_3000.dat 1 0 0

#2(limited OpenMP with sequential when needed) (4 is our optimal number for livethreads)
./main_cpu_program ../data/dt_2_10000_3000.dat 2 4 0

#Hybrid MPI with OpenMP (this to actually see the boost, needs to run on cluster with 2 nodes)
mpirun -np 2 main_cpu_program ../data/dt_2_10000_3000.dat 3 4 0


