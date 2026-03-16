reset

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 0.5
set style line 2 lc rgb '#e0ad04' lt 1 lw 2 pt 7 ps 0.5
set style line 3 lc rgb '#375f03' lt 1 lw 2 pt 7 ps 0.5

set term pdf size 5, 5
set output "leapfrog.pdf"

set xrange [*:*]
set yrange [-2:2]

set multiplot layout 3, 1 title "Leapfrog" 

plot 'log' using 0:1 t "X" with line ls 1
plot 'log' using 0:2 t "V" with line ls 2
plot 'log' using 0:3 t "F" with line ls 3

unset multiplot
#pause -1


