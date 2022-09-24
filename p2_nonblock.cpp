#include <math.h>
#include <mpi.h>
#include <math.h>
#include <stdio.h>

#include <iostream>

using namespace std;

const bool P = false; // toggle operation output
const bool R = false; // toggle buffer result output

int main(int argc, char **argv)
{
  int numtasks, rank, next, prev, tag1 = 1, tag2 = 2;
  size_t N = (size_t)pow(10,1);
  // size_t N = 0;

  double sbuf[N], rbuf_prev[N], rbuf_next[N];
  double t0, t1, t3, t4;
  MPI_Status stats[2];
  MPI_Request reqs[2];
  MPI_Init(&argc, &argv);                    // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  // Total number of processors

  prev = rank - 1;
  next = rank + 1;
  if (rank == 0) prev = numtasks - 1;
  if (rank == (numtasks - 1)) next = 0;

  srand(123456 + rank);
  for (size_t i = 0; i < N; i++)
  {
    sbuf[i] = (double)rand()/RAND_MAX;
  }
  if (R) cout <<"rank "<< rank << " SEND: " << sbuf[0] << endl;

  t0 = MPI_Wtime();

  if (numtasks > 1){
    t3 = MPI_Wtime();

    MPI_Irecv(rbuf_prev, N, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(sbuf, N, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD, &reqs[1]);
      
    MPI_Waitall(2, reqs, stats);
    t4 = MPI_Wtime();
  }
  else{
    memcpy(rbuf_prev, sbuf,N*sizeof*sbuf);
  }

  // cout << "rank: " << rank << " latency (ms): " << 1000 * abs(t4 - t3) << endl;
  t1 = MPI_Wtime();

  if (rank == 0)
    // cout << "Processors: " << numtasks << ", N: " << N
    //      << ", Latency (ms): " << 1000 * (t1 - t0) << endl;
    cout << "+ " <<1000 * (t1 - t0);

  if (R)
    cout << "rank " << rank << ", PREV: " << rbuf_prev[0] << endl;
  // if (R)
  //   cout << "rank " << rank << " NEXT first/last: " << rbuf_next[0] << ", "
  //        << rbuf_next[N - 1] << endl;

  MPI_Finalize();
}