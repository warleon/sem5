#include <math.h>
#include <mpi.h>
#include <stdio.h>

#include <iostream>

using namespace std;

const bool P = true;  // toggle operation output
const bool R = true;  // toggle buffer result output

int main(int argc, char **argv) {
  int numtasks, rank, next, prev, tag1 = 1, tag2 = 2;
  size_t N = (size_t)pow(10, 5);
  double sbuf[N], rbuf_prev[N], rbuf_next[N];
  double t0, t1;
  MPI_Status stats[4];
  MPI_Init(&argc, &argv);                    // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  // Total number of processors

  prev = rank - 1;
  next = rank + 1;
  if (rank == 0) prev = numtasks - 1;
  if (rank == (numtasks - 1)) next = 0;

  srand(123456 + rank);
  for (size_t i = 0; i < N; i++) {
    sbuf[i] = (double)rand() / RAND_MAX;
  }
  if (R)
    cout << "rank " << rank << " SEND first/last: " << sbuf[0] << ", "
         << sbuf[N - 1] << endl;

  t0 = MPI_Wtime();

  if (numtasks > 1) {
    //* SEND RECEIVE COUNTERCLOCKWISE
    if (rank == 0) {
      if (P) cout << "sent to prev: " << prev << endl;
      // send inicial, desbloquea el ultimo rank
      // block hasta concluir receive en el ultimo rank
      MPI_Send(sbuf, N, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD);
    }

    if (P) cout << "receiving in rank: " << rank << endl;
    // block hasta recibir send del rank next
    MPI_Recv(rbuf_next, N, MPI_DOUBLE, next, tag2, MPI_COMM_WORLD, &stats[0]);
    if (P) cout << "received rank: " << rank << endl;

    if (rank != 0) {
      if (P) cout << "sent to prev: " << prev << endl;
      // desbloquea rank prev, block hasta concluir receive de prev
      MPI_Send(sbuf, N, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD);
    }

    /*----------------------------------------------------*/

    //* SEND RECEIVE CLOCKWISE
    if (rank == 0) {
      if (P) cout << "sent to next: " << next << endl;
      // send inicial, desbloquea el rank 1
      // block hasta concluir receive en el rank 1
      MPI_Send(sbuf, N, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD);
    }

    if (P) cout << "receiving in rank: " << rank << endl;
    // block hasta recibir send del rank prev
    MPI_Recv(rbuf_prev, N, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD, &stats[0]);
    if (P) cout << "received rank: " << rank << endl;

    if (rank != 0) {
      if (P) cout << "sent to next: " << next << endl;
      // desbloquea rank next, block hasta concluir receive de next
      MPI_Send(sbuf, N, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD);
    }

  } else {
    memcpy(rbuf_prev, sbuf, N * sizeof *sbuf);
    memcpy(rbuf_next, sbuf, N * sizeof *sbuf);
  }

  if (P) cout << "finished rank: " << rank << endl;
  t1 = MPI_Wtime();

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