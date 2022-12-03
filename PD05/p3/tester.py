from subprocess import Popen, PIPE

pr = 4
iterations = 10

# print("\nUsing N:", N)
print("   processors", "  |\t","average(s)")
print("-------------------------------------")

for pr_i in range(1,pr+1):
  program_path = f'mpirun -np {pr_i} ./a.out'
  avg = 0
  for i in range(iterations):
    p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
    result = p.stdout.readline().strip().decode()
    avg += float(result)
  avg = avg / iterations
  print("\t",pr_i, "\t|  ",f'{avg:.20f}')
