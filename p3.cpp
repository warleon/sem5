#include <math.h>
#include <mpi.h>
#include <stdio.h>
int main(void) {
  int my_rank, comm_sz;

  const size_t N{1 << 5};
  double mat[N][N]{}, vec[N]{}, x[N]{};

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // rellenar el vector y matriz
  if (!my_rank) {
    for (size_t i = 0; i < N; i++) {
      vec[i] = (double)rand() / RAND_MAX;
      for (size_t j = 0; j < N; j++) mat[i][j] = (double)rand() / RAND_MAX;
      std::cout << vec[i] << " ";
    }
    std::cout << "\n";
  }

  // comunicacion
  const size_t my_rowsBegin{(N / comm_sz) * my_rank};
  const size_t my_workload{N / comm_sz};
  const size_t my_rowsEnd{my_rowsBegin + N / my_workload};
  double my_mat[my_workload][N]{};
  MPI_Bcast(vec, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(mat, N * N, MPI_DOUBLE, my_mat, my_workload * N, MPI_DOUBLE, 0,
              MPI_COMM_WORLD);

  // Procesar
  for (size_t i = 0; i < my_workload; i++) {
    for (size_t j = 0; j < N; j++) x[i] += my_mat[i][j] * vec[j];
  }

  // comunicacion
  MPI_Gather(x, my_workload, MPI_DOUBLE, vec, my_workload, MPI_DOUBLE, 0,
             MPI_COMM_WORLD);
  if (!my_rank) {
    for (size_t i = 0; i < N; i++) {
      std::cout << vec[i] << " ";
    }
    std::cout << "\n";
  }

  MPI_Finalize();
  return 0;
}
