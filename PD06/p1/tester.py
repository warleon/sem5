from subprocess import Popen, PIPE
import pandas as pd
import numpy as np
import tabulate

pr = 4

print("\nUsing:", pr, "processes")
print("-------------------------------------")

df = pd.DataFrame(columns=["x","temperature"])
for pr_i in [pr]:
  program_path = f'mpirun -np {pr_i} ./a.out'
  p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
  for ln in iter(p.stdout.readline,""):
    line = ln.strip().decode()
    if line == "":
      break
    x = line.split(", ")
    x = [float(i) for i in x]
    df = pd.concat([df, pd.DataFrame([x], columns=["x","temperature"])])
  

  df = df.sort_values(by=['x'])
  df.index = np.arange(1, len(df) + 1)
  print(df.to_markdown())
