# Script de gnuplot para graficar polinomios de Lagrange

set terminal pngcairo size 1400,900 enhanced font 'Arial,12'
set output 'polinomios_lagrange.png'

set xlabel 'x' font ',14'
set ylabel 'y' font ',14'
set title 'Polinomios de Lagrange Individuales' font ',16'
set grid
set key top left box

# Estilos de línea
set style line 1 lc rgb '#e41a1c' lt 1 lw 2
set style line 2 lc rgb '#377eb8' lt 1 lw 2
set style line 3 lc rgb '#4daf4a' lt 1 lw 2
set style line 4 lc rgb '#984ea3' lt 1 lw 2
set style line 5 lc rgb '#ff7f00' lt 1 lw 2
set style line 6 lc rgb '#ffff33' lt 1 lw 2

plot 'lagrange_L0.txt' using 1:2 with lines ls 1 title 'L_{0}(x)', \
     'lagrange_L1.txt' using 1:2 with lines ls 2 title 'L_{1}(x)', \
     'lagrange_L2.txt' using 1:2 with lines ls 3 title 'L_{2}(x)', \
     'lagrange_P.txt' using 1:2 with lines lw 3 lc rgb 'black' dashtype 2 title 'P(x) = {/Symbol S} y_i L_i(x)', \
     'nodos.txt' using 1:2 with points pt 7 ps 2 lc rgb 'red' title 'Puntos originales'
