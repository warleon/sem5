#include <iostream>
#include <cmath>
#include <ctime>

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
  int i, j, j_min = 0, j_max = 400, tag, n = 8;
  double k = 0.002;
  double tiempo, dt, tmax = 10.0, tmin = 0.0, tnew;
  double u[n], unew[n], x[n], dx; // u+1
  double x_max = 1.0, x_min = 0.0;

  dt = (tmax - tmin) / (double)(j_max - j_min);
  printf("dt: %f\n", dt);
  dx = (x_max - x_min) / (double)(n - 1);

  x[0] = 0;

  for (i = 1; i < n; i++)
  {
    x[i] = x[i - 1] + dx;
  }

  std::cout << "created x\n";
  for (int i = 0; i < n; i++)
  {
    std::cout << x[i] << " ";
  }
  std::cout << "\n";

  //  Inicializacion.
  tiempo = tmin;
  u[0] = 0.0;
  for (i = 1; i <= n; i++)
    u[i] = inicial(x[i], tiempo);
  u[n + 1] = 0.0;

  std::cout << "created u\n";
  for (int i = 0; i <= n; i++)
  {
    std::cout << u[i] << " ";
  }
  std::cout << "\n";

  // Valores de temperatura al siguiente intervalo de tiempo
  for (j = 1; j <= j_max; j++)
  {
    tnew += dt;
    // printf("tnew: %f\n",tnew);

    // actualizar temperatura
    for (i = 1; i <= n; i++)
    {
      std::cout << "prev: " << u[i - 1] << "\n";
      std::cout << "curr: " << u[i] << "\n";
      std::cout << "next: " << u[i + 1] << "\n";
      unew[i] = u[i] + (dt * k / dx / dx) * (u[i - 1] - 2.0 * u[i] + u[i + 1]);
      std::cout << "out: " << unew[i] << "\n";
    }
    std::cout << "tnew:" << tnew << "\n";
    std::cout << "x[n]: " << x[n] << "\n";
    // aplicar condiciones de frontera
    unew[1] = frontera(x[1], tnew);
    unew[n] = frontera(x[n], tnew);

    // Actualizar tiempo y temperatura
    tiempo = tnew;
    // printf("tiempo: %f\n",tiempo);

    for (i = 1; i <= n; i++)
    {
      u[i] = unew[i];
      if (j == j_max)
        printf("%f %f %f\n", tiempo, x[i], u[i]);
    }
  }

  return 0;
}
