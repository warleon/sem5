/*  MPI_Reduce  */

#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
    int me, numprocs;
    int data, res;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // inicializacion en proceso 0
    if (me == 0){
        data = 3;
    };

    // Broadcast
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    cout << "rank: " << me << ", data:" << data << endl;
    
    // Reduce
    MPI_Reduce(&data, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    // resultado en proceso 0
    if (me == 0){
        cout << "results: " << res << endl;
    };

    MPI_Finalize();
}
