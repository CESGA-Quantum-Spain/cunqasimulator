#!/bin/bash

#SBATCH -J test            
#SBATCH -o test_%j.o       
#SBATCH -e test_%j.e      
#SBATCH -n 5                           
#SBATCH -N 1
#SBATCH -c 1                            
#SBATCH --time 00:30:00                 
#SBATCH --mem-per-cpu=15G        

module load qmio/hpc gcc/12.3.0 nlohmann_json/3.11.3 impi/2021.13.0 

mpirun -n 5 ./../build_mpi/examples/mpi_example