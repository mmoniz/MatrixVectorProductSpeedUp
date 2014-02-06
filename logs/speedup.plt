set title "Speed Up for Different Values of P for Matrix x Vector"
set xlabel "Number of Processes (p)"
set ylabel "Speed Up"
set logscale x 2
set key left
plot "speedup1.txt" title 'N=1951' with lines, \
"speedup2.txt" title 'N=3902' with lines, \
"speedup3.txt" title 'N=7804' with lines