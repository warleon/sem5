#!/bin/bash
# Nombre del job:
#SBATCH --job-name=p3_2
# Nombre del archivo de salida:
#SBATCH --output=p3_2-4t.txt
# Numero de tasks:
#SBATCH --ntasks=1
# Numero de CPUs por task:
#SBATCH --cpus-per-task=4
# Límite de tiempo de 10 min:
#SBATCH --time=10:00
./a.out < 4
