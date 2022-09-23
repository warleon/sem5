#include <mpi.h>
#include <iostream>
#include <math.h>
#define K 1024
int main(int argc, char **argv)
{
  int mpi_rank, mpi_size;
  MPI_Status stat;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  double t0 = 0.0, tf = 0.0;
  int n, iter = 3;
  double local_sum, global_sum = 0, result, error;
  const int max_iter = 20;
  const double pi = 3.141592653589793;

  // se itera max_iter veces
  while (iter <= max_iter)
  {
    t0 = MPI_Wtime();
    local_sum = 0, global_sum = 0, result = 0;

    // datos en el maestro
    if (mpi_rank == 0)
    {
      n = pow(2, iter);
    }

    // Broadcast
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // suma local
    for (int i = (n / mpi_size * mpi_rank) + 1;
         i <= (n / mpi_size * (mpi_rank + 1)); i++)
      local_sum += (4 / (1 + pow((i - 0.5) / n, 2)));

    // std::cout << "pi: " << result << std::endl;

    // Reduce (suma global)
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // resultados en el maestro
    if (mpi_rank == 0)
    {
      result = global_sum / n;
      error = pi - result;
      tf = MPI_Wtime();
      printf("Valor de PI con n = 2^%d: %1.6f, error %e, tiempo: %1.3f ms \n",
             iter, result, error, 1000 * (tf - t0));
    }
    iter++;
  } // while

  MPI_Finalize(); // Exit MPI
}
