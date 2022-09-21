# include <iostream>
# include <mpi.h>

int main ( int argc, char *argv[] )
{
  int rank,N=3,M=6,stride=6;
  int p;

  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
  MPI_Comm_size ( MPI_COMM_WORLD, &p );
  MPI_Status status;

double A[N][M];
/*
| 0 | 1 | 2 | 3 | 4 |  5 |
| 0 | 1 | 2 | 3 | 4 |  5 |
| 0 | 1 | 2 | 3 | 4 |  5 |
*/


MPI_Datatype newtype;
MPI_Type_vector(2, 3, stride, MPI_DOUBLE, &newtype); 
MPI_Type_commit(&newtype); 

if (rank==0) {
printf("rank: %d \n",rank);
for (int i=0; i<N;i++) { 
for (int j=0; j<M;j++) {
	A[i][j]=j;
	printf("%f ",A[i][j]);}
printf("\n");
}
MPI_Send(&A[0][1], 1, newtype, 1, 0, MPI_COMM_WORLD); 
//MPI_Send(&A, 1, newtype, 1, 0, MPI_COMM_WORLD); 
} 

else if (rank==1) {
double subdominio[N][M];
MPI_Recv(&subdominio, 1, newtype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
//MPI_Recv(&subdominio[0][1], 1, newtype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
//MPI_Recv(&v, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
printf("rank: %d \n",rank);
for (int i=0; i<N;i++) { 
for (int j=0; j<M;j++) {
	printf("%f ",subdominio[i][j]);}
printf("\n");
}
}
MPI_Type_free(&newtype);

}
