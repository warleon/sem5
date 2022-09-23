#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	int ierr, count, rank, size;
	MPI_Init(&argc, &argv);
	MPI_Finalize();
}
