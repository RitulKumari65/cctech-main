set title '3D Cuboid'
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set grid
set mouse
set view 60, 30
splot 'cuboid_data.dat' with lines lc rgb 'green'
pause -1 'Press any key to exit'
