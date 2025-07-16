#!/bin/bash

#SBATCH -J test            
#SBATCH -o test_%j.o       
#SBATCH -e test_%j.e      
#SBATCH -n 4                           
#SBATCH -N 4
#SBATCH -c 64                            
#SBATCH --time 00:30:00                 
#SBATCH --mem-per-cpu=15500MB        

module load qmio/hpc gcc/12.3.0 nlohmann_json/3.11.3 impi/2021.13.0 

mpirun -n $SLURM_NTASKS ./../build/examples/mpi_example