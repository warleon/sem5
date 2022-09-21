#ifdef _PARALLEL
#include <mpi.h>
#endif
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
	int data, me;

#ifdef _PARALLEL
    int numprocs,res;	
#endif

#ifdef _SERIE
    me = 0;
	data = 5.;
	cout<<"Proceso "<<me<<", data: "<<data<<endl;
#endif

#ifdef _PARALLEL
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    if(me==0)
	    data = 5.;

    MPI_Bcast(&data,1,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
        
	MPI_Reduce(&data, &res, 1, MPI_INT, MPI_SUM,
               0, MPI_COMM_WORLD);
//	if (me==0) {
		cout<<"Proceso "<<me<<", data: "<<data<<", res: "<<res<<endl;
  //  }
    MPI_Finalize();
#endif
    return 0;
}
