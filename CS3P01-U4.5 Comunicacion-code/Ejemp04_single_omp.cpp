#include <iostream>
using namespace std;

#ifdef _OMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

int main()
{
   int n = 10;
   int i, a, b[n];
#ifdef _OMP
   omp_set_num_threads(8);
#endif
   for (i=0; i<n; i++)
       b[i] = -1;
#pragma omp parallel 
{
   #pragma omp single 
       	{
      a = 10;
      b[omp_get_thread_num()]=a;
      cout<<"constructor Single ejecutado por el hilo "<<omp_get_thread_num()<<endl;
	}   
} 
cout<<"Fin de la region en paralelo: "<<endl;
   for (i=0; i<n; i++)
   cout<<"b["<<i<<"]"<<"= "<<b[i]<<endl;

}
