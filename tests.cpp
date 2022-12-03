#include "mpi.h"
#include "math.h"
#include "stdio.h"

#define k 20

int calculo(float A[], int n)
{
  int i, cont = 0;
  float promedio = 0, suma_cuadrados = 0, desviacion = 0;
  int mpi_rank, mpi_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  float A_local[n / mpi_size];

  MPI_Scatter(&A[0], n / mpi_size, MPI_FLOAT, &A_local[0], n / mpi_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

  for (i = 0; i < n / mpi_size; i++)
  {
    promedio += A_local[i];
  }
  promedio = promedio / n;

  MPI_Allreduce(MPI_IN_PLACE, &promedio, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  for (i = 0; i < n / mpi_size; i++)
  {
    suma_cuadrados += pow(A_local[i] - promedio, 2);
  }
  desviacion = suma_cuadrados / (n - 1);

  MPI_Allreduce(MPI_IN_PLACE, &desviacion, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
  if (mpi_rank ==0){
    desviacion = sqrt(desviacion);
  }
  MPI_Bcast(&desviacion, 1, MPI_FLOAT,0,MPI_COMM_WORLD);

  for (i = 0; i < n / mpi_size; i++)
  {
    if (A_local[i] < desviacion)
    {
      cont++;
    }
  }

  MPI_Allreduce(MPI_IN_PLACE, &cont, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  if (mpi_rank==0)
    printf("res parallel: %d\n", cont);
  return cont;
}

int calculoxd(float A[], int n)
{
  int i, cont = 0;
  float promedio = 0, suma_cuadrados = 0, desviacion = 0;
  int mpi_rank, mpi_size;

  for (i = 0; i < n; i++)
  {
    promedio += A[i];
  }
  promedio = promedio / n;

  for (i = 0; i < n; i++)
  {
    suma_cuadrados += pow(A[i] - promedio, 2);
  }
  desviacion = suma_cuadrados / (n - 1);
  desviacion = sqrt(desviacion);

  for (i = 0; i < n; i++)
  {
    if (A[i] < desviacion)
    {
      cont++;
    }
  }
  printf("res sequential: %d\n", cont);
  return cont;
}

int main(int argc, char **argv)
{
  int mpi_rank, mpi_size;
  // MPI_Status stat;
  // MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  int N = pow(2, k);
  // if (mpi_rank == 0)
  // {
  float matrix[N];
  for (int i = 0; i < N; i++)
  {
    matrix[i] = (float)rand()/(float)(RAND_MAX);
  }

  // }
  calculoxd(matrix, N);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  calculo(matrix, N);

  MPI_Finalize();
  return 0;
}
