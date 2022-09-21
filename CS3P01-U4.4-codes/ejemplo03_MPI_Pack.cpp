#include <iostream>
#include <mpi.h>
#define TAM_BUFFER 100
#define TAG 0
#define DEST 1
#define SOURCE 0

int main(int argc, char *argv[]) {
   int   rank,p;
   char  buffer[TAM_BUFFER];
   int   posicion;

   float a, b;
   int   n;
 
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size ( MPI_COMM_WORLD, &p );
   MPI_Status  status;
   if ( rank == SOURCE ) {
      a = 10.3;
      b = 11.2;
      n = 4;
      posicion = 0;
      MPI_Pack(&a,1,MPI_FLOAT,buffer,TAM_BUFFER,&posicion,MPI_COMM_WORLD);
      MPI_Pack(&b,1,MPI_FLOAT,buffer,TAM_BUFFER,&posicion,MPI_COMM_WORLD);
      MPI_Pack(&n,1,MPI_INT,buffer,TAM_BUFFER,&posicion,MPI_COMM_WORLD);
      MPI_Send(buffer,TAM_BUFFER,MPI_PACKED,DEST,TAG,MPI_COMM_WORLD);
   }
  else {
      MPI_Recv(buffer,TAM_BUFFER,MPI_PACKED,SOURCE,TAG,
      MPI_COMM_WORLD, &status);
      posicion = 0;
      MPI_Unpack(buffer,TAM_BUFFER,&posicion,&a,1,MPI_FLOAT,
      MPI_COMM_WORLD);
      printf("   a = %f\n",a);
      MPI_Unpack(buffer,TAM_BUFFER,&posicion,&b,1,MPI_FLOAT,
      MPI_COMM_WORLD);
      printf("   b = %f\n",b);
      MPI_Unpack(buffer,TAM_BUFFER,&posicion,&n,1,MPI_INT,
      MPI_COMM_WORLD);
      printf("   n = %d\n",n);
   }
   MPI_Finalize();
}

