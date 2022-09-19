set terminal png
set output 'ej02.png'
set log x
set log y
set xlabel 'np'
set ylabel 'T (ms)'
set xtics (2,3,4,5,6,7,8,9,10)
set title 'Cálculo de Promedio, Tiempo de ejecución vs np'
set key top right 


plot 'ej2_data.txt' using 1:2 w l title "Tcomm", '' u 1:3 w l title "Tcomp", '' u 1:4 title "Ttot"
