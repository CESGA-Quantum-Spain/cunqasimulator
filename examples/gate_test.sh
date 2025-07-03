#!/bin/bash

#SBATCH -J test_gate            
#SBATCH -o test_gate_%j.o       
#SBATCH -e test_gate_%j.e      
#SBATCH -n 1                           
#SBATCH -N 1
#SBATCH -c 64                            
#SBATCH --time 00:30:00                 
#SBATCH --mem-per-cpu=15500MB        

module load qmio/hpc gcc/12.3.0 nlohmann_json/3.11.3 impi/2021.13.0 

srun ./../build_mpi/examples/gate_test