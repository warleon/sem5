
#include <mpi.h>
#include <stdio.h>

#include <atomic>
#include <iostream>
#include <vector>

using namespace std;

std::atomic<int> msgRecv, msgSend;
std::vector<bool> sflag1, sflag2;

int main(int argc, char **argv)
{
  std::cout << "GAAAAAAAAAAA" << std::endl;
  int numtasks, rank, next, prev;
  //   int buf[2];
  int tag1 = 1, tag2 = 2, N = 1024;
  int sbuf[N], rbuf[N];
  double t0, t1;
  int pendingMessage;
  //   MPI_Status stats[4];
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processors

  //   MPI_Request reqs[4];

  prev = (rank - 1 + numtasks) % numtasks;
  next = (rank + 1 + numtasks) % numtasks;

  if (int(sflag1.size()) != numtasks)
  {
    sflag1.resize(numtasks);
  }
  if (int(sflag2.size()) != numtasks)
  {
    sflag2.resize(numtasks);
  }
  t0 = MPI_Wtime();
  do
  {
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &pendingMessage,
               MPI_STATUS_IGNORE);
    if (pendingMessage)
    {
      std::cout << "pendingMessage if finished" << std::endl;
      MPI_Recv(rbuf, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      msgRecv++;

    }
    if (msgSend != msgRecv && !sflag1[rank])
    {
      std::cout << "sflag1 if finished" << std::endl;
      msgSend++;
      sflag1[rank] = 1;
      MPI_Send(sbuf, N, MPI_INT, next, tag1, MPI_COMM_WORLD);

    }
    if (msgSend != msgRecv && !sflag2[rank])
    {
      std::cout << "sflag2 if finished" << std::endl;
      msgSend++;
      sflag2[rank] = 1;
      MPI_Send(sbuf, N, MPI_INT, prev, tag2, MPI_COMM_WORLD);
    }
    std::cout << "a: " << msgRecv << std::endl;

  } while (msgSend == 2 * numtasks && msgRecv == 2 * numtasks);

  t1 = MPI_Wtime();

  if (rank == 0)
    cout << "Tiempo (s): " << t1 - t0 << endl;
  std::cout << "smurfeados" << std::endl;

  MPI_Finalize();
}