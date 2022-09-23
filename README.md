# paralela sem 5
 

```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope # disable wsl vader error
mpic++ ejemplo01.cpp
mpirun -np <procesos> ./a.out
```