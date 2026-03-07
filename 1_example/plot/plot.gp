reset

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.1
set style line 2 lc rgb '#e0ad04' lt 1 lw 2 pt 7 ps 0.1
set style line 3 lc rgb '#375f03' lt 1 lw 2 pt 7 ps 0.1

set term pdf size 5, 5
set output "leapfrog.pdf"

set xrange [*:*]
set yrange [-2:2]

set multiplot layout 3, 1 title "Leapfrog" 
unset key
plot for [i=1:500] sprintf('output/X_%d.0.tex', i) every ::0::0 using (i):1 with linespoints ls 1

set title "Velocity (V)"
plot for [i=1:500] sprintf('output/V_%d.5.tex', i) every ::0::0 using (i):1 with linespoints ls 2

set title "Force (F)"
plot for [i=1:500] sprintf('output/F_%d.0.tex', i) every ::0::0 using (i):1 with linespoints ls 3

unset multiplot
#pause -1


