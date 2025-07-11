set title '3D Sphere'
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set grid
set mouse
set view 60, 30
splot 'sphere_data.dat' with lines lc rgb 'blue'
pause -1 'Press any key to exit'
