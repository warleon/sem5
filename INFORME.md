#    Ejercicio 1

Se implementaron dos versiones diferentes del algoritmo, una realizando primero un calculo counter clockwise de los prev y luego clockwise de los next, y el otro usando mod 2 de una manera similar. Para este analisis se utiliza el primero.

Se realizó un promedio con los tiempos obtenidos luego de 100 iteraciones por cada número de procesos. Se aprecia que el código NO es escalable, tanto por el comportamiento de la gráfica como por el diseño del mismo, debido a que cada proceso que se añade, genera una mayor complejidad a la hora del mensajeo entre el proceso previo y el siguiente.

![](https://i.imgur.com/tcpL8Sv.png)


| <!-- -->  |<!-- -->  |<!-- --> |<!-- -->  | <!-- -->| <!-- -->|
|-----------|----------|---------|----------|------|---------|
| #p        | 1        | 2       | 4        | 6    | 8       |
| time (ms) | 0.225573 | 1.25991 | 2.247489 | 4.07 | 9.36273 |


# Ejercicio 2

Con la regresión lineal de las gráficas, se estima un valor de latencia de 2.35ms con blocking y de 0.74ms sin blocking. Si bien es un número grande en el caso con bloqueo, es razonable debido al comportamiento necesario para evitar deadlocks. La mayor parte de esta latencia se presenta en P0:

![](https://i.imgur.com/Fi8VgwS.jpg)

Se presentan gráficas para ambos casos:

![](https://i.imgur.com/i3u6s9i.png)

![](https://i.imgur.com/NMsh4pn.png)


Se compara la latencia estimada con la latencia con N = 0, obteniendo el porcentaje de error

|  | Block Latency | NonBlock Latency |
| -------- | -------- | -------- |
|  Estimation    | 2.351467383931392     | 0.7441643220649893     |
|  Calculated    | 1.4782326923076925     | 0.5650409999999999     |
|  Error    | **59.07%**     | **31.70%**     |


*¿Cual resultado considera representa mejor el tiempo de latencia?* 

Se considera que que estas estimaciones de latencia parecen ser mas precisas con respecto a un diseño sin bloqueo, debido a que el tiempo de espera necesario para prevenir casos de deadlock toman la mayor parte del tiempo de mensaje. Observando los datos, también se observaba que mensajes del tipo Non Blocking tomaban con mayor tiempo que el mesajeo con bloqueo, pero de igual manera al no tener que manejar una logica adicional de espera, la latencia total de los p procesos era considerablemente menor para estos casos, ademas de ser mas preciso de estimar mediante la regresión.

# Ejercicio 3

Se logró implementar un código MPI para este problema.

Los costos computacional y de comunicación se pueden medir como los anteriores casos.

Tej vs procesos:
| <!-- -->  |<!-- -->  |<!-- --> |<!-- -->  | <!-- -->| <!-- -->|
|-----------|----------|---------|----------|------|---------|
| #p        | 1        | 2       | 4        | 6    | 8       |
| time (ms) | 0.0235 | 0.1678 |  0.2892 | 0.347 | 0.4565 |

![](https://i.imgur.com/XZWvTEd.png)

Escalabilidad: