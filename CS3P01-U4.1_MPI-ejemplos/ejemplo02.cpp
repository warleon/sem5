#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{

  int my_rank, size;

  MPI_Init(&argc, &argv);                  /*inicia MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*RANK DEL PROCESO */
  MPI_Comm_size(MPI_COMM_WORLD, &size);    /* numero de procesos*/

  MPI_Finalize(); /* salida MPI */
}
