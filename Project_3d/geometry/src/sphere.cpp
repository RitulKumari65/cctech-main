#include "../include/sphere.h"
#include <fstream>
#include <cmath>

Sphere::Sphere(double x, double y, double z, double radius, int resolution)
    : x(x), y(y), z(z), radius(radius), resolution(resolution) {}

void Sphere::saveData() {
    std::ofstream out("sphere_data.dat");
    
    for (int i = 0; i <= resolution; i++) {
        double theta = i * M_PI / resolution;
        for (int j = 0; j <= 2 * resolution; j++) {
            double phi = j * M_PI / resolution;
            double x1 = x + radius * sin(theta) * cos(phi);
            double y1 = y + radius * sin(theta) * sin(phi);
            double z1 = z + radius * cos(theta);
            out << x1 << " " << y1 << " " << z1 << "\n";
        }
        out << "\n";
    }

    out.close();
}

void Sphere::plotInteractive() {
    saveData();
    
    std::ofstream script("plot_sphere.gnu");
    script << "set title '3D Sphere'\n"
           << "set xlabel 'X'\n"
           << "set ylabel 'Y'\n"
           << "set zlabel 'Z'\n"
           << "set grid\n"
           << "set mouse\n"
           << "set view 60, 30\n"
           << "splot 'sphere_data.dat' with lines lc rgb 'blue'\n"
           << "pause -1 'Press any key to exit'\n";
    script.close();

    system("gnuplot plot_sphere.gnu");
}
