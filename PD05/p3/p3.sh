#!/bin/bash

# Nombre del job:
#SBATCH -J p3
# Nombre de la partición:
#SBATCH -p investigacion
#SBATCH --output=p3-4t.txt
# Número de nodos:
#SBATCH -N 4 
# Número de tasks por nodo:
#SBATCH --tasks-per-node=1

module load mpich/4.0
mpirun -np 4 ./a.out
module unload mpich/4.0


