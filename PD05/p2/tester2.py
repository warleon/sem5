from subprocess import Popen, PIPE

pr = 4
iterations = 100
program_path = "./a.out"

print("   processors", "  |\t","average(s)")
print("-------------------------------------")

avg = 0
buckets = [0] * pr
for i in range(iterations):
  p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
  p.stdin.write((str(pr)+"\n").encode())
  p.stdin.flush()
  result = p.stdout.readline().strip().decode()
  res_split = result.split(",")
  res_split.pop()
  for x in res_split:
    cur = x.split(":")
    buckets[int(cur[0])] += float(cur[1])

for i in range(len(buckets)):
  buckets[i] = buckets[i] / iterations 
  print("\t",i, "\t|  ",f'{buckets[i]:.20f}')
