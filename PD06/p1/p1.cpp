#include <iostream>
#include <cmath>
#include <ctime>
#include <mpi.h>

double frontera(double x, double tiempo)
{
  double limite;
  if (x < 0.5)
    limite = 100.0 + 10.0 * sin(tiempo);
  else
    limite = 75.0;
  return limite;
}

double inicial(double x, double tiempo)
{
  double limite;
  limite = 95.0;
  return limite;
}

int main(int argc, char *argv[])
{
  int my_rank, comm_sz;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int i, j, j_min = 0, j_max = 400, tag, n = 10;
  int n_com = n * comm_sz;
  double k = 0.002;
  double tiempo, dt, tmax = 10.0, tmin = 0.0, tnew;
  double u[n_com], x[n_com], dx;
  double sub_u[n], unew[n], sub_x[n];
  double x_max = 1.0, x_min = 0.0;

  dt = (tmax - tmin) / (double)(j_max - j_min);
  dx = (x_max - x_min) / (double)(n_com - 1);

  if (!my_rank)
  {
    x[0] = 0;
    for (i = 1; i < n_com; i++)
    {
      x[i] = x[i - 1] + dx;
    }
    //  Inicializacion.
    tiempo = tmin;
    for (i = 0; i < n_com; i++)
      u[i] = inicial(x[i], tiempo);
  }

  MPI_Scatter(u, n, MPI_DOUBLE, sub_u,
              n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(x, n, MPI_DOUBLE, sub_x,
              n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Valores de temperatura al siguiente intervalo de tiempo
  double next_u, prev_u, next_u_recv, prev_u_recv;
  for (j = 1; j <= j_max; j++)
  {
    tnew += dt;

    if (my_rank != 0)
      MPI_Send(&sub_u[0], 1, MPI_DOUBLE, my_rank - 1, 1, MPI_COMM_WORLD);

    if (my_rank != comm_sz - 1)
      MPI_Recv(&next_u_recv, 1, MPI_DOUBLE, my_rank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int fst = 0;
    int lst = n;

    for (i = fst; i < lst; i++)
    {
      if (i < 1)
      {
        if (my_rank != 0)
        {
          MPI_Recv(&prev_u_recv, 1, MPI_DOUBLE, my_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          prev_u = prev_u_recv;
        }
        else
          prev_u = 0;
      }
      else
        prev_u = sub_u[i - 1];

      if (i + 1 >= n)
      {
        if (my_rank != comm_sz - 1)
          next_u = next_u_recv;
        else
          next_u = 0;
      }
      else
        next_u = sub_u[i + 1];

      // calc unew[i]
      unew[i] = sub_u[i] + (dt * k / dx / dx) * (prev_u - 2.0 * sub_u[i] + next_u);

      // send last if needed
      if (i + 1 >= n)
      {
        if (my_rank != comm_sz - 1)

          MPI_Send(&unew[i], 1, MPI_DOUBLE, my_rank + 1, 1, MPI_COMM_WORLD);
      }
    }

    if (my_rank == 0)
      unew[0] = frontera(sub_x[0], tnew);

    if (my_rank == comm_sz - 1)
      unew[n - 1] = frontera(0, tnew);

    // Actualizar tiempo y temperatura
    tiempo = tnew;

    // update and print if last iteration
    for (i = 0; i < n; i++)
    {
      sub_u[i] = unew[i];
      if (j == j_max)
        printf("%f, %f\n", sub_x[i], sub_u[i]);
    }
  }

  MPI_Finalize();

  return 0;
}
