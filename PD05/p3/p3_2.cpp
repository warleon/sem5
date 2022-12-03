
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <iomanip>

int THREADS;

int main()
{
  int my_rank, comm_sz;
  const size_t N{1 << 8};
  std::cin >> THREADS;

  omp_set_num_threads(THREADS);
  double t1, t2;
  double mat[N][N]{}, vec[N]{}, x[N]{};

  // rellenar el vector y matriz
  for (size_t i = 0; i < N; i++)
  {
    vec[i] = (double)rand() / RAND_MAX;
    for (size_t j = 0; j < N; j++)
      mat[i][j] = (double)rand() / RAND_MAX;
  }

  t1 = omp_get_wtime();

#pragma omp parallel default(none) \
    shared(vec, mat, x)            \
        firstprivate(N)            \
            num_threads(THREADS)
  {
    int i, y;
#pragma omp for schedule(static, 16)
    for (y = 0; y < N; y++)
    {
      double results = 0;
      for (i = 0; i < N; i++)
      {
        results += vec[i] * mat[i][y];
      }
      x[y] = results;
    }
  }

  t2 = omp_get_wtime();
  printf("%f8", (t2 - t1));

  return 0;
}