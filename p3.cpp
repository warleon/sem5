#include <mpi.h>
#include <stdio.h>
int main(void) {
  int my_rank, comm_sz;

  const size_t N{1 << 5};
  int mat[N][N]{}, vec[N]{}, x[N]{};

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // rellenar el vector y matriz
  if (!my_rank) {
  }

  // comunicacion
  size_t my_rowsBegin = (N / comm_sz) * my_rank;
  size_t my_workload = N / comm_sz;
  size_t my_rowsEnd = my_rowsBegin + N / my_workload;
  MPI_Bcast(vec, N, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(mat, N * N, MPI_INT, &mat[my_rowsBegin][0], my_workload * N,
              MPI_INT, 0, MPI_COMM_WORLD);

  // Procesar
  for (size_t i = 0; i < my_workload; i++) {
    for (size_t j = 0; j < N; j++) x[i] += mat[my_rowsBegin + i][j] * vec[j];
  }

  // comunicacion
  MPI_Gather(x, my_workload, MPI_INT, vec, my_workload, MPI_INT, 0,
             MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}
