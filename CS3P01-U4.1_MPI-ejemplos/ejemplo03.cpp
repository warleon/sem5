#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{

  int my_rank, size;

  MPI_Init(&argc, &argv);                  /*inicio MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*determina el rank del proceso*/
  MPI_Comm_size(MPI_COMM_WORLD, &size);    /*determina el numero de procesos*/

  MPI_Finalize(); /* salida MPI */
}
