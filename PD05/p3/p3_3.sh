#!/bin/bash

# Nombre del job:
#SBATCH -J p3_3
# Nombre de la partición:
#SBATCH -p investigacion
#SBATCH --output=p3_3-4t.txt
# Número de nodos:
#SBATCH -N 4 
# Número de tasks por nodo:
#SBATCH --tasks-per-node=1
mpirun -np 4 ./a.out < 4