
# compilar paralelo
g++ p2.cpp -DPARALLEL_EXEC -fopenmp

## flag para usar diferentes sorters
g++ p2.cpp -DPARALLEL_EXEC -DUSE_SORTERS -fopenmp && ./a.out

# compilar secuencial
g++ p2.cpp 

echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope # disable wsl vader error


# P3
 ./a.out < 4


 mpic++ -fopenmp p3_3.cpp && mpirun -np 4 ./a.out < 4


 g++ p3_2.cpp -fopenmp && ./a.out < 4

 mpic++ p3.cpp && mpirun -np 4 ./a.out