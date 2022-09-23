
#include <math.h>
#include <mpi.h>
#include <stdio.h>

#include <atomic>
#include <iostream>
#include <vector>

using namespace std;

const bool P = true;  // toggle operation output
const bool R = true;  // toggle buffer result output

int main(int argc, char **argv) {
  int numtasks, rank, next, prev;
  //   int buf[2];
  int tag1 = 1, tag2 = 2;
  // size_t N = (size_t)pow(10, 1);
  size_t N = (size_t)2;

  double sbuf[N], rbuf_prev[N], rbuf_next[N];
  //  N = 1024;
  // int sbuf[N], rbuf[N];
  MPI_Status stats[4];

  double t0, t1;
  int pendingMessage;
  int msgRecv, msgSend;
  bool sendNext = false;
  bool sendBefore = false;
  //   MPI_Status stats[4];
  MPI_Init(&argc, &argv);                    // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  // Total number of processors

  //   MPI_Request reqs[4];

  prev = (rank - 1 + numtasks) % numtasks;
  next = (rank + 1 + numtasks) % numtasks;

  srand(123456 + rank);
  for (size_t i = 0; i < N; i++) {
    sbuf[i] = (double)rand() / RAND_MAX;
  }
  if (R)
    cout << "rank " << rank << " SEND first/last: " << sbuf[0] << ", "
         << sbuf[N - 1] << endl;

  t0 = MPI_Wtime();
  if (rank % 2) {
    MPI_Recv(rbuf_prev, N, MPI_INT, prev, MPI_ANY_TAG, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(rbuf_next, N, MPI_INT, next, MPI_ANY_TAG, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  } else {
    MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);
    MPI_Send(sbuf, N, MPI_INT, prev, tag2, MPI_COMM_WORLD);
  }
  if (!(rank % 2)) {
    MPI_Recv(rbuf_prev, N, MPI_INT, prev, MPI_ANY_TAG, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(rbuf_next, N, MPI_INT, next, MPI_ANY_TAG, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  } else {
    MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);
    MPI_Send(sbuf, N, MPI_INT, prev, tag2, MPI_COMM_WORLD);
  }
  if (P) cout << "finished rank: " << rank << endl;

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