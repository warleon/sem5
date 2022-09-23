#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;

const bool P = true; // toggle printear output

int main(int argc, char **argv)
{
  int numtasks, rank, next, prev, tag1 = 1, tag2 = 2, N = 1024;
  int sbuf[N], rbuf[N];
  double t0, t1;
  MPI_Status stats[4];
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processors

  prev = rank - 1;
  next = rank + 1;
  if (rank == 0)
    prev = numtasks - 1;
  if (rank == (numtasks - 1))
    next = 0;

  t0 = MPI_Wtime();
  
  //* SEND RECEIVE COUNTERCLOCKWISE
  if (rank == 0)
  {
    if (P) cout << "sent to prev: " << prev << endl;
    // send inicial, desbloquea el ultimo rank
    // block hasta concluir receive en el ultimo rank
    MPI_Send(sbuf, N, MPI_INT, prev, tag2, MPI_COMM_WORLD);
  }

  if (P) cout << "receiving in rank: " << rank << endl;
  // block hasta recibir send del rank next
  MPI_Recv(rbuf, N, MPI_INT, next, tag2, MPI_COMM_WORLD, &stats[0]);
  if (P) cout << "received rank: " << rank << endl;

  if (rank != 0)
  {
    if (P) cout << "sent to prev: " << prev << endl;
    // desbloquea rank prev, block hasta concluir receive de prev
    MPI_Send(sbuf, N, MPI_INT, prev, tag2, MPI_COMM_WORLD);
  }
  
/*----------------------------------------------------*/

  //* SEND RECEIVE CLOCKWISE
  if (rank == 0)
  {
    if (P) cout << "sent to next: " << next << endl;
    // send inicial, desbloquea el rank 1
    // block hasta concluir receive en el rank 1
    MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);
  }

  if (P) cout << "receiving in rank: " << rank << endl;
  // block hasta recibir send del rank prev
  MPI_Recv(rbuf, N, MPI_INT, prev, tag1, MPI_COMM_WORLD, &stats[0]);
  if (P) cout << "received rank: " << rank << endl;

  if (rank != 0)
  {
    if (P) cout << "sent to next: " << next << endl;
    // desbloquea rank next, block hasta concluir receive de next
    MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);
  }

  if (P) cout << "finished rank: " << rank << endl;
  t1 = MPI_Wtime();

  if (rank == 0)
    cout << "Tiempo (s): " << t1 - t0 << endl;

  MPI_Finalize();
}