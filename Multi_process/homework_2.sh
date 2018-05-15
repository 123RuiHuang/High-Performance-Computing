#!/bin/sh
#SBATCH -J rhhq7  # give the job a custom name

#SBATCH -o results_hw2_8_1m-%j.out  # give the job output a custom name

#SBATCH -e error_hw2_8_1m-%j.log #give the job error file a custom name

#SBATCH -t 0-02:00  # two hour time limit#SBATCH -N 1  # number of nodes

#SBATCH -n 1  # number of cores (AKA tasks)

#SBATCH -c 8  # number of cpus per task

#SBATCH --mem 5G# Commands here run only on the first core

make clean
make

srun homework_2 ../../../../../data/scottgs/hpc_1m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_1m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_1m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_1m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_1m.csv 8

srun homework_2 ../../../../../data/scottgs/hpc_2m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_2m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_2m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_2m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_2m.csv 8

srun homework_2 ../../../../../data/scottgs/hpc_3m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_3m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_3m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_3m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_3m.csv 8

srun homework_2 ../../../../../data/scottgs/hpc_4m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_4m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_4m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_4m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_4m.csv 8

srun homework_2 ../../../../../data/scottgs/hpc_5m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_5m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_5m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_5m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_5m.csv 8

srun homework_2 ../../../../../data/scottgs/hpc_6m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_6m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_6m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_6m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_6m.csv 8


srun homework_2 ../../../../../data/scottgs/hpc_7m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_7m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_7m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_7m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_7m.csv 8


srun homework_2 ../../../../../data/scottgs/hpc_8m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_8m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_8m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_8m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_8m.csv 8

srun homework_2 ../../../../../data/scottgs/hpc_9m.csv 1
srun homework_2 ../../../../../data/scottgs/hpc_9m.csv 2
srun homework_2 ../../../../../data/scottgs/hpc_9m.csv 4
srun homework_2 ../../../../../data/scottgs/hpc_9m.csv 6
srun homework_2 ../../../../../data/scottgs/hpc_9m.csv 8