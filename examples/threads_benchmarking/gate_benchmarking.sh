#!/bin/bash

#SBATCH -J gate_bench            
#SBATCH -o gate_bench_%j.o       
#SBATCH -e gate_bench_%j.e      
#SBATCH -n 1                           
#SBATCH -N 1
#SBATCH -c 64                            
#SBATCH --time 10:00:00                 
#SBATCH --mem-per-cpu=15500MB        

module load qmio/hpc gcc/12.3.0 cmake/3.27.6 nlohmann_json/3.11.3 impi/2021.13.0 hpcx-ompi ninja/1.9.0

srun ./../../build_mpi/examples/gate_bench