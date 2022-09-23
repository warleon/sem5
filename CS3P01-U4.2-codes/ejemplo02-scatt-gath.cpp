#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv)
{
  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int globaldata[size];
  int localdata;
  int i;

  // inicializacion en proceso 0
  if (rank == 0)
  {
    cout << "globaldata before: " << endl;
    for (int i = 0; i < size; i++)
    {
      globaldata[i] = 2 * i + 1;
      cout << globaldata[i] << " ";
    }
  cout << endl;
  }

  // Comunicacion (scatter)
  MPI_Scatter(&globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Computo
  localdata = localdata * 2;
  // Comunicacion (gather)
  MPI_Gather(&localdata, 1, MPI_INT, &globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // resultado en proceso 0
  if (rank == 0)
  {
    cout << "globaldata after: " << endl;
    for (i = 0; i < size; i++)
    {
      cout << globaldata[i] << " ";
    }
    cout << endl;
  };

  MPI_Finalize();
  return 0;
}
