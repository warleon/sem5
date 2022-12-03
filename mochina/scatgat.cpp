#include <mpi.h>
#include <iostream>
using namespace std;

#define N 128
#define M 128

int main(int argc, char **argv)
{
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int matrix[N][M];
    int vector[M];
    int result[N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                matrix[i][j] = i + j;
            }
            vector[i] = i;
        }
    }

    int loc_matrix[N][M];

    MPI_Scatter(&matrix[0][0], (N * M) / size, MPI_INT, &loc_matrix[0][0], (N * M) / size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vector[0], M, MPI_INT, 0, MPI_COMM_WORLD);

    int loc_result[N];
    for (int i = 0; i < N / size; i++) {
        loc_result[i] = 0;
        for (int j = 0; j < M; j++) {
            loc_result[i] += loc_matrix[i][j] * vector[j];
        }
    }

    MPI_Gather(&loc_result[0], N / size, MPI_INT, &result[0], N / size, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            cout << result[i] << " ";
        }
    }

    MPI_Finalize();
    return 0;
}