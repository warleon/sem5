# include <iostream>
# include <mpi.h>

int main ( int argc, char *argv[] )
{
int p, rank, i, j, mb,M=6,N=6;
        double A[M][N], B[M][N], Al[M][N], Bl[M][N];
        MPI_Init(&argc, &argv);
 	MPI_Comm_size(MPI_COMM_WORLD, &p);
     	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if (rank==0) {
for (int i=0; i<N;i++) {
for (int j=0; j<M;j++) {
        A[i][j]=j;
        B[i][j]=2*j;
    //    printf("%f ",A[i][j]);
}
//printf("\n");
}
for (int i=0; i<N;i++) {
for (int j=0; j<M;j++) {
        printf("%f ",A[i][j]);
}
printf("\n");
}
for (int i=0; i<N;i++) {
for (int j=0; j<M;j++) {
        printf("%f ",B[i][j]);
}
printf("\n");
}

}

     MPI_Datatype cyclic_row;
     mb = M/p;
     MPI_Type_vector(mb, N, p*N, MPI_DOUBLE, &cyclic_row);
     MPI_Type_commit(&cyclic_row);
     if (rank==0) {
       for (i=1;i<p;i++) {
         MPI_Send(&A[i][0], 1, cyclic_row, i, 0, MPI_COMM_WORLD);
         MPI_Send(&B[i][0], 1, cyclic_row, i, 1, MPI_COMM_WORLD);
       }
       MPI_Sendrecv(A, 1, cyclic_row, 0, 0, Al, mb*N, MPI_DOUBLE,
                    0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       MPI_Sendrecv(B, 1, cyclic_row, 0, 1, Bl, mb*N, MPI_DOUBLE,
                    0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     } else {
       MPI_Recv(Al, mb*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
       MPI_Recv(Bl, mb*N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
}

 for (i=0;i<mb;i++)
 for (j=0;j<N;j++)
	  Al[i][j] += Bl[i][j];

 if (rank==0) {
       for (i=1;i<p;i++)
         MPI_Recv(&A[i][0], 1, cyclic_row, i, 3, MPI_COMM_WORLD,
                  MPI_STATUS_IGNORE);
       MPI_Sendrecv(Al, mb*N, MPI_DOUBLE, 0, 3, A, 1, cyclic_row,
                    0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     }
     else MPI_Send(Al, mb*N, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
     MPI_Type_free(&cyclic_row);


if (rank==0) {
for (int i=0; i<N;i++) {
for (int j=0; j<M;j++) {
        printf("%f ",A[i][j]);
}
printf("\n");
}
}
     MPI_Finalize();

}
