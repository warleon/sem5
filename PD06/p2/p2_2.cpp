#include <omp.h>

#include <iostream>
#include <algorithm>

#define SORT_NAME int
#define SORT_TYPE int
#define SORT_CMP(x, y) ((x) - (y))
#include "./sort.h"

using namespace std;

#define LIMIT 1000

void imprimir_array(int A[], int N);
void ParallelSort(int A[], int N);
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
  ParallelSort(numeros, size);
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

void imprimir_array_range(int A[], int beg, int end)
{
  int i;
  cout << "[";
  for (i = beg; i < end; i++)
    cout << A[i] << " ";
  cout << "]\n";
}

void mergeInPlace(int A[], int start, int end, int m_start, int m_end)
{
  int L[end - start + 2];
  int R[m_end - m_start + 2];
  for (int i = start; i <= end; i++)
  {
    L[i - start] = A[i];
  }
  for (int i = m_start; i <= m_end; i++)
  {
    R[i - m_start] = A[i];
  }
  L[end - start + 1] = INT32_MAX;
  R[m_end - m_start + 1] = INT32_MAX;

  int l = 0;
  int r = 0;
  int k = start;

  while (k <= (m_end))
  {
    if (L[l] <= R[r])
    {
      A[k] = L[l];
      // cout<<L[l]<<"\n";
      l++;
    }
    else
    {
      A[k] = R[r];
      // cout<<R[r]<<"\n";
      r++;
    }
    k++;
  }
}

int getStart(int id, int p, int N)
{
  N--;
  int start = (N * id) / p;
  if (id != 0)
    start++;
  return start;
}

int getEnd(int id, int p, int N)
{
  N--;
  int end = (N * (id + 1)) / p;
  if (id == p - 1)
    end = N;
  return end;
}

void ParallelSort(int A[], int N)
{
// fase de ordenamiento individual
#pragma omp parallel
  {
    int id = omp_get_thread_num();
    int p = omp_get_num_threads();
    int start = getStart(id, p, N);
    int end = getEnd(id, p, N);

    int left_start = getStart(id + 1, p, N);
    int left_end = getEnd(id + 1, p, N);
    int right_start = getStart(id + 1, p, N);
    int right_end = getEnd(id + 1, p, N);

    int_quick_sort(&A[start], end + 1 - start);

#pragma omp barrier

    for (int i = 0; i < p; i++)
    {
      if (i % 2)
      { // impar
        if (id % 2)
        {
          if (id + 1 < p)
          {
            // printf("thread_id: %d compare left with %d\n", id, id + 1);
            mergeInPlace(A, start, end, left_start, left_end);
          }
        }
      }
      else
      { // par
        if (!(id % 2))
        {
          // printf("thread_id: %d compare right with %d\n", id, id + 1);
          mergeInPlace(A, start, end, right_start, right_end);
        }
      }
#pragma omp barrier 
   
    }
  }
}
