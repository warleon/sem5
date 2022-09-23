#include <mpi.h>
using namespace std;

int main(int argc, char *argv[])
{
    int i;
    int rank, size, buf;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << "Peekaboo 1! desde " << rank << ", buf: " << size << endl;

    if (rank == 0)
    {
        buf = 1;
        for (i = 1; i < size; i++)
        {
            cout << "before send to " << i << endl;
            MPI_Send(&buf, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            cout << "send to:" << i << endl;
        }
    }
    else
    {
        cout << "before receive in  "<< rank << endl;
        MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        cout << "received in "<< rank << endl;
    }

    // cout << "Peekaboo 2! desde " << rank << ", buf: " << buf << endl;

    MPI_Finalize();

    return 0;
}