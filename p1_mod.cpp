
#include <math.h>
#include <mpi.h>
#include <stdio.h>

#include <atomic>
#include <iostream>
#include <vector>

using namespace std;

const bool P = false; // toggle operation output
const bool R = false; // toggle buffer result output

int main(int argc, char **argv)
{
  int numtasks, rank, next, prev;
  int tag1 = 1, tag2 = 2;
  size_t N = (size_t)pow(10, 5);

  double sbuf[N], rbuf_prev[N], rbuf_next[N];
  double t0, t1;

  MPI_Init(&argc, &argv);                    // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  // Total number of processors

  prev = (rank - 1 + numtasks) % numtasks;
  next = (rank + 1 + numtasks) % numtasks;

  srand(123456 + rank);
  for (size_t i = 0; i < N; i++)
  {
    sbuf[i] = (double)rand() / RAND_MAX;
  }
  if (R)
    cout << "rank " << rank << " SEND first/last: " << sbuf[0] << ", "
         << sbuf[N - 1] << endl;

  t0 = MPI_Wtime();
  if (rank % 2)
  {
    MPI_Recv(rbuf_prev, N, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(rbuf_next, N, MPI_DOUBLE, next, tag2, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }
  else
  {
    MPI_Send(sbuf, N, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD);
    MPI_Send(sbuf, N, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD);
  }
  if (!(rank % 2))
  {

    MPI_Recv(rbuf_prev, N, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(rbuf_next, N, MPI_DOUBLE, next, tag2, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }
  else
  {
    MPI_Send(sbuf, N, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD);
    MPI_Send(sbuf, N, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD);
  }
  if (P)
    cout << "finished rank: " << rank << endl;

  t1 = MPI_Wtime();

  // if (rank == 0) cout << "Tiempo (s): " << t1 - t0 << endl;

  if (rank == 0)
    cout << "Processors: " << numtasks << ", N: " << N
         << ", Time (ms): " << 1000 * (t1 - t0) << endl;

  if (R)
    cout << "rank " << rank << " PREV first/last: " << rbuf_prev[0] << ", "
         << rbuf_prev[N - 1] << endl;
  if (R)
    cout << "rank " << rank << " NEXT first/last: " << rbuf_next[0] << ", "
         << rbuf_next[N - 1] << endl;

  MPI_Finalize();
}