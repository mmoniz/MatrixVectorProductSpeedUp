set title "Efficency for Different Values of P for Matrix x Vector"
set xlabel "Number of Processes (p)"
set ylabel "Efficency"
set logscale x 2
set key right
plot "eff1.txt" title 'N=1951' with lines, \
"eff2.txt" title 'N=3902' with lines, \
"eff3.txt" title 'N=7804' with lines