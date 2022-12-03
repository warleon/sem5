#include <iostream>
#include <omp.h>
using namespace std;

int main(){
  int j, k, jota, ka;
  ka = 0;
  jota = 0;

  #pragma omp parallel for private(j, k) reduction(max:ka) reduction(max:jota)
  for (k = 1; k <= 128; k++)
    for (j = 1; j <= 256; j++)
    {
      jota = j;
      ka = k;
    }

  cout << "jota: " << jota << ", ka: " << ka << endl;
}