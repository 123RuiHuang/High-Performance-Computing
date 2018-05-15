set terminal png nocrop enhanced
# log y
set output 'hw1.1.logy.png'
set log y
set ylabel 'Seconds'
set xlabel 'Approximate Operations'
plot "p1.f.data" using 2:1 title 'f' with linespoints lw 2, \
        "p1.m1.data" using 2:1 title 'm1' with linespoints lw 2, \
	"p1.m2.data" using 2:1 title 'm2' with linespoints lw 2, \
	"p1.m3.data" using 2:1 title 'm3' with linespoints lw 2, \
	"p1.m4.data" using 2:1 title 'boost' with linespoints lw 2

