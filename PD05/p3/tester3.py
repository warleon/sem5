from subprocess import Popen, PIPE

pr_mpi = [1,2,4]
pr_omp = [1,4,8]
pr_cnt = 2
iterations = 1

print("   processors", "  |\t","average(s)")
print("-------------------------------------")

for pr_i in range(0,pr_cnt+1):
  program_path = f'mpirun -np {pr_mpi[pr_i]} ./a.out'
  avg = 0
  for i in range(iterations):
    p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
    p.stdin.write((str(pr_omp[pr_i])+"\n").encode())
    p.stdin.flush()
    result = p.stdout.readline().strip().decode()
    avg += float(result)
  avg = avg / iterations
  print("\t","mpi,omp:",pr_mpi[pr_i],pr_omp[pr_i], "\t|  ",f'{avg:.20f}')

