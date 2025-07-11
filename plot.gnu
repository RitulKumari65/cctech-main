set title '3D Line with Interactive Rotation'
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set grid
set mouse
set view 60, 30
splot 'line_data.dat' with linespoints lc rgb 'red' pt 7
pause -1 'Press any key to exit'
