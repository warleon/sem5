#ifdef PARALLEL_EXEC
#include <omp.h>
#endif
#include <math.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

#define SORT_NAME float
#define SORT_TYPE float
#define SORT_CMP(x, y) ((x) - (y))
#include "./sort.h"

void bucketSort(float *arr, int &n, int bucket_size)
{
  // Crear buckets
  std::vector<float> bucket[bucket_size];
#ifdef PARALLEL_EXEC
  omp_lock_t *lock = new omp_lock_t[bucket_size];
#pragma omp parallel for
  for (int k = 0; k < bucket_size; ++k)
    omp_init_lock(&lock[k]);
#endif
  int i, j, bucket_index, index = 0;

  // asignar elementos a los buckets
  for (i = 0; i < n; ++i)
  {
    bucket_index = bucket_size * arr[i] / 1000;
#ifdef PARALLEL_EXEC
    omp_set_lock(&lock[bucket_index]);
#endif
    bucket[bucket_index].push_back(arr[i]);
#ifdef PARALLEL_EXEC
    omp_unset_lock(&lock[bucket_index]);
#endif
  }

// ordenar buckets
#pragma omp parallel for
  for (j = 0; j < bucket_size; ++j)
  {
    // #ifdef PARALLEL_EXEC
    // printf("Bucket: %d in thread: %d\n", j, omp_get_thread_num());
    // #endif
#ifdef USE_SORTERS
    double start;
    double end;
    start = omp_get_wtime();
    // to use the lib
    float temp[bucket[j].size()];
    std::copy(bucket[j].begin(), bucket[j].end(), temp);
    switch (j)
    {
    case 0:
      float_quick_sort(temp, bucket[j].size());
      break;
    case 1:
      float_merge_sort(temp, bucket[j].size());
      break;
    case 2:
      float_heap_sort(temp, bucket[j].size());
      break;
    default:
      float_shell_sort(temp, bucket[j].size());
      break;
    }
    end = omp_get_wtime();
    // printf("Bucket %d sorting took %f seconds\n", omp_get_thread_num(), end - start);
    printf("%d:%0.20f,", omp_get_thread_num(), end - start);
    bucket[j] = std::vector<float>(temp, temp + bucket[j].size());
#else
    sort(bucket[j].begin(), bucket[j].end());
#endif
  }
  // Concatenar buckets en arr[]
  for (i = 0; i < bucket_size; i++)
  {
    for (j = 0; j < bucket[i].size(); j++)
      arr[index++] = bucket[i][j];
  }
#ifdef PARALLEL_EXEC
#pragma omp parallel for
  for (int k = 0; k < bucket_size; ++k)
    omp_destroy_lock(&lock[k]);
  delete[] lock;
#endif
}

int main(int argc, char *argv[])
{
  int THREADS = 1;
  std::cin >> THREADS;
#ifdef PARALLEL_EXEC
  omp_set_num_threads(THREADS);
#endif
  int i, n = pow(2, 19);
  float *randArray;

  srand((int)time(0));

  randArray = new float[n];

  for (int i = 0; i < n; ++i)
    randArray[i] = (float)rand() / (float)(RAND_MAX / 999.0);

  // for (i = 0; i < n; ++i)
  //   printf("%1.2f, ", randArray[i]);
  // printf("\n");

  // ordenar array en buckets
  double s = omp_get_wtime();
  bucketSort(randArray, n, THREADS);
  double e = omp_get_wtime();
  // printf("BucketSort sorting took %f seconds\n", e - s);
  // printf("%.20f\n", e - s);


  // for (i = 0; i < n; ++i)
  //   printf("%1.2f, ", randArray[i]);

  delete[] randArray;
}