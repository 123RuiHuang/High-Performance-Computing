#!/bin/bash
module add openmpi/openmpi-3.0.0
make
if [ -f "outputFile1.txt" ]; 
then rm output*
fi
mpirun -np 2 ./hw4 ../../../../../data/scottgs/book outputFile1.txt

mpirun -np 4 ./hw4 ../../../../../data/scottgs/book outputFile2.txt

mpirun -np 6 ./hw4 ../../../../../data/scottgs/book outputFile3.txt

mpirun -np 8 ./hw4 ../../../../../data/scottgs/book outputFile4.txt

mpirun -np 10 ./hw4 ../../../../../data/scottgs/book outputFile5.txt

mpirun -np 12 ./hw4 ../../../../../data/scottgs/book outputFile6.txt



make clean

