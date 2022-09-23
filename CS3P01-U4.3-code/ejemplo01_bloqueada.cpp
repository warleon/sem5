#include <mpi.h>
#include <stdio.h>

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  int numtasks, rank, next, prev, buf[2], tag1 = 1, tag2 = 2, N = 1024;
  int sbuf[N], rbuf[N];
  double t0, t1;
  MPI_Status stats[4];
  MPI_Init(&argc, &argv);                    // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  //tal number of processors
  MPI_Request reqs[4];

  prev = rank - 1;
  next = rank + 1;
  if (rank == 0) prev = numtasks - 1;
  if (rank == (numtasks - 1)) next = 0;

  t0 = MPI_Wtime();
  MPI_Recv(rbuf, N, MPI_INT, prev, tag1, MPI_COMM_WORLD, &stats[0]);
  MPI_Recv(rbuf, N, MPI_INT, next, tag2, MPI_COMM_WORLD, &stats[0]);

  MPI_Send(sbuf, N, MPI_INT, prev, tag2, MPI_COMM_WORLD);
  MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);

  t1 = MPI_Wtime();

  if (rank == 0) cout << "Tiempo (s): " << t1 - t0 << endl;

  MPI_Finalize();
}
// nofuncaprofexddd