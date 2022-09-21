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

mb = M/p;
     for (i=0;i<mb;i++) {
       MPI_Scatter(&A[i*p][0], N, MPI_DOUBLE, &Al[i][0], N,
                   MPI_DOUBLE, 0, MPI_COMM_WORLD);
       MPI_Scatter(&B[i*p][0], N, MPI_DOUBLE, &Bl[i][0], N,
                   MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
 for (i=0;i<mb;i++)
 for (j=0;j<N;j++)
	  Al[i][j] += Bl[i][j];

 for (i=0;i<mb;i++)
       MPI_Gather(&Al[i][0], N, MPI_DOUBLE, &A[i*p][0], N,
                   MPI_DOUBLE, 0, MPI_COMM_WORLD);

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
