#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>

using namespace std;

#define N 1000
#define s 100

int main(int argc, char **argv)
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(1, s);

  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int EduardoNumero, NumeroAmigo;
  int comps;

  if (rank == 0)
  {
    EduardoNumero = dist(mt);
    // comentar para tester
    printf("Numero de Eduardo: %d\n", EduardoNumero); 
  }

  // Broadcast
  MPI_Bcast(&EduardoNumero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  bool found = false;
  for (int i = 0; i < N; i++)
  {
    NumeroAmigo = dist(mt);
    comps = NumeroAmigo == EduardoNumero;

    int compbuf = 0;
    // Reduce
    MPI_Reduce(&comps, &compbuf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);   

    // check 
    if (rank == 0)
    {
      if (compbuf == 4)
      {
        found = true;
        break;
      }
    }
  }
  if (rank == 0)
  {
    if (found)
      printf("found\n");
    else
      printf("not found\n");
  }
  
  MPI_Finalize();
  return 0;
}