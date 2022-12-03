from subprocess import Popen, PIPE

pr = 4
iterations = 500
program_path = f'mpirun -np {pr} ./a.out'

print("\nUsing Iterations:", iterations)
count_founds = 0
for i in range(iterations):
  p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
  p.stdin.flush()
  result = p.stdout.readline().strip().decode()
  if result == "found":
    count_founds += 1
  print("iter:",i,"result:", result)
prob = count_founds / iterations
print(f'probability ={prob:.20f}')
