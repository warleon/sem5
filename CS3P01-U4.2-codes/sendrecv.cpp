#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
	int rank, size, number;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout << "Peekaboo 1! desde " << rank << ", number: " << number << endl;

	if (rank == 0)
	{
		number = -1;
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		cout << "if desde " << rank << ", number: " << number << endl;

	}
	else if (rank == 1)
	{
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "else if desde " << rank << ", number: " << number << endl;
	}

	cout << "Peekaboo 2! desde " << rank << ", number: " << number << endl;

	MPI_Finalize();

	return 0;
}