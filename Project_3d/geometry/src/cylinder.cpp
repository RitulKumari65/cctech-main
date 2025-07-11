#include "../include/cylinder.h"
#include <fstream>
#include <cmath>

Cylinder::Cylinder(double x, double y, double z, double radius, double height, int segments)
    : x(x), y(y), z(z), radius(radius), height(height), segments(segments), 
      rotationX(0), rotationY(0), rotationZ(0) {}

void Cylinder::setRotation(double rotX_deg, double rotY_deg, double rotZ_deg) {
    rotationX = rotX_deg * M_PI / 180.0;
    rotationY = rotY_deg * M_PI / 180.0;
    rotationZ = rotZ_deg * M_PI / 180.0;
}

std::vector<Point3D> Cylinder::getDrawable() {
    std::vector<Point3D> vertices;

    // Generate top & bottom circle points
    for (int i = 0; i < segments; i++) {
        double theta = 2 * M_PI * i / segments;
        double x_offset = radius * cos(theta);
        double y_offset = radius * sin(theta);

        // Bottom circle
        vertices.push_back({x + x_offset, y + y_offset, z});
        // Top circle
        vertices.push_back({x + x_offset, y + y_offset, z + height});
    }

    // Apply rotation to all points
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

void Cylinder::saveData() {
    std::vector<Point3D> vertices = getDrawable();
    std::ofstream out("cylinder_data.dat");

    // Connect bottom and top circles
    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments;

        // Bottom circle
        out << vertices[2 * i].x << " " << vertices[2 * i].y << " " << vertices[2 * i].z << "\n";
        out << vertices[2 * next].x << " " << vertices[2 * next].y << " " << vertices[2 * next].z << "\n";
        out << "NaN NaN NaN\n";  // 🟢 Prevent unwanted connections

        // Top circle
        out << vertices[2 * i + 1].x << " " << vertices[2 * i + 1].y << " " << vertices[2 * i + 1].z << "\n";
        out << vertices[2 * next + 1].x << " " << vertices[2 * next + 1].y << " " << vertices[2 * next + 1].z << "\n";
        out << "NaN NaN NaN\n";  

        // Side edges
        out << vertices[2 * i].x << " " << vertices[2 * i].y << " " << vertices[2 * i].z << "\n";
        out << vertices[2 * i + 1].x << " " << vertices[2 * i + 1].y << " " << vertices[2 * i + 1].z << "\n";
        out << "NaN NaN NaN\n";  
    }

    out.close();
}

void Cylinder::plotInteractive() {
    saveData();
    
    std::ofstream script("plot_cylinder.gnu");
    script << "set title '3D Cylinder'\n"
           << "set xlabel 'X'\n"
           << "set ylabel 'Y'\n"
           << "set zlabel 'Z'\n"
           << "set grid\n"
           << "set mouse\n"
           << "set view 60, 30\n"
           << "splot 'cylinder_data.dat' with lines lc rgb 'blue'\n"
           << "pause -1 'Press any key to exit'\n";
    script.close();

    system("gnuplot plot_cylinder.gnu");
}
