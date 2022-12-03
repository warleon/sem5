#include <omp.h>

#include <iostream>
using namespace std;

#define LIMIT 1000

void imprimir_array(int A[], int N);
void Sort(int A[], int N);
void Swap(int *X, int *Y);
void leer_array(int array[], int *limite, char *filename);

int numeros[LIMIT]; // tamanho maximo del array
int size = LIMIT;

int main(int argc, char *argv[])
{
  if (argc < 2)
  { // no hay archivo => exit
    exit(1);
  }
  omp_set_num_threads(8);

  // se leen los numeros de un archivo
  leer_array(numeros, &size, argv[1]);
  // se imprime el array
  imprimir_array(numeros, size);
  // se ordena el  array
  Sort(numeros, size);
  // se imprime el array ordenado
  printf("\nArray ordenado:\n");
  imprimir_array(numeros, size);

  return 0;
}

void leer_array(int array[], int *size, char *filename)
{
  FILE *opened; // file
  char buf[30];
  int numero;      // numero de elementos leidos
  int counter = 0; // counter for elems in file

  opened = fopen(filename, "r");

  while (!feof(opened) &&
         counter < *size)
  {                            // lee hasta EOF o muchos elementos
    fgets(buf, 30, opened);    // lee una linea, max 30 caracteres
    numero = atoi(buf);        // convierte a entero
    array[counter++] = numero; // dar valores al array
    if (counter >= *size)      // muchos numeros para el array
      cout << "...muchos numeros para el array\n";
  }
  fclose(opened);

  *size = --counter; // ajusta el tamaño
  cout << *size << " numeros leidos" << endl;
}

void imprimir_array(int A[], int N)
{
  int i;
  cout << "[";
  for (i = 0; i < N; i++)
    cout << A[i] << " ";
  cout << "]\n";
}
void CompareExchange(int *X, int *Y)
{
  int t = *X ^ *Y;
  *X = t ^ max(*X, *Y); // X is minimum now
  *Y = t ^ *X;          // Y is maximum now
}

void setMaxAtEnd(int A[], int N)
{
  int m = 0;
  for (int i = 0; i < N; i++)
  {
    m = A[i] > A[m] ? i : m;
  }
  Swap(&A[m], &A[N - 1]);
}

void ParallelSort(int A[], int N)
{
  for (int i = 0; i < N; i++)
  {
#pragma omp parallel for
    for (int j = 0; j < N / 2; j++)
    {
      std::cout<< omp_get_thread_num() << "\n";

      if (i % 2)
      { // impar
        CompareExchange(&A[j * 2 + 1], &A[j * 2 + 2]);
      }
      else
      { // par
        CompareExchange(&A[j * 2], &A[j * 2 + 1]);
      }
    }
  }
}

void Sort(int A[], int N)
{
  if (N % 2)
  { // impar
    ParallelSort(A, N);
  }
  else
  {
    setMaxAtEnd(A, N);
    ParallelSort(A, N - 1);
  }
}

void Swap(int *X, int *Y)
{
  int temp = *X;
  *X = *Y;
  *Y = temp;
}