#include "../include/cuboid.h"
#include <fstream>
#include <cmath>

Cuboid::Cuboid(double x, double y, double z, double l, double b, double h)
    : x(x), y(y), z(z), l(l), b(b), h(h), rotationX(0), rotationY(0), rotationZ(0) {}

void Cuboid::setRotation(double rotX_deg, double rotY_deg, double rotZ_deg) {
    rotationX = rotX_deg * M_PI / 180.0;
    rotationY = rotY_deg * M_PI / 180.0;
    rotationZ = rotZ_deg * M_PI / 180.0;
}

std::vector<Point3D> Cuboid::getDrawable() {
    std::vector<Point3D> vertices = {
        {x, y, z},                // 0: left-bottom-front  
        {x + l, y, z},            // 1: right-bottom-front
        {x + l, y + b, z},        // 2: right-top-front
        {x, y + b, z},            // 3: left-top-front
        {x, y, z + h},            // 4: left-bottom-back
        {x + l, y, z + h},        // 5: right-bottom-back
        {x + l, y + b, z + h},    // 6: right-top-back
        {x, y + b, z + h}         // 7: left-top-back
    };

    for (auto& v : vertices) {
        double x_ = v.x - x;
        double y_ = v.y - y;
        double z_ = v.z - z;

        // Rotation around X-axis
        double y1 = y_ * cos(rotationX) - z_ * sin(rotationX);
        double z1 = y_ * sin(rotationX) + z_ * cos(rotationX);

        // Rotation around Y-axis
        double x2 = x_ * cos(rotationY) + z1 * sin(rotationY);
        double z2 = -x_ * sin(rotationY) + z1 * cos(rotationY);

        // Rotation around Z-axis
        double x3 = x2 * cos(rotationZ) - y1 * sin(rotationZ);
        double y3 = x2 * sin(rotationZ) + y1 * cos(rotationZ);

        v.x = x3 + x;
        v.y = y3 + y;
        v.z = z2 + z;
    }

    return vertices;
}

void Cuboid::saveData() {
    std::vector<Point3D> vertices = getDrawable();
    std::ofstream out("cuboid_data.dat");

    int edges[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Back face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting edges
    };

    for (auto& edge : edges) {
        out << vertices[edge[0]].x << " " << vertices[edge[0]].y << " " << vertices[edge[0]].z << "\n";
        out << vertices[edge[1]].x << " " << vertices[edge[1]].y << " " << vertices[edge[1]].z << "\n";

        // 🟢 Insert NAN to break connections between faces
        out << "NaN NaN NaN\n";  
    }

    out.close();
}

void Cuboid::plotInteractive() {
    saveData();
    
    std::ofstream script("plot_cuboid.gnu");
    script << "set title '3D Cuboid'\n"
           << "set xlabel 'X'\n"
           << "set ylabel 'Y'\n"
           << "set zlabel 'Z'\n"
           << "set grid\n"
           << "set mouse\n"
           << "set view 60, 30\n"
           << "splot 'cuboid_data.dat' with lines lc rgb 'green'\n"
           << "pause -1 'Press any key to exit'\n";
    script.close();

    system("gnuplot plot_cuboid.gnu");
}

// Cube is just a special case of a Cuboid with equal sides
Cube::Cube(double x, double y, double z, double side)
    : Cuboid(x, y, z, side, side, side) {}
