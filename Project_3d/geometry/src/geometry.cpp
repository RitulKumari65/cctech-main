#include "../include/geometry.h"

Point3D::Point3D(double x, double y, double z) : x(x), y(y), z(z) {}

Point3D RotatingLine::rotateX(const Point3D& p, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return {p.x, p.y * cosA - p.z * sinA, p.y * sinA + p.z * cosA};
}

Point3D RotatingLine::rotateY(const Point3D& p, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return {p.x * cosA + p.z * sinA, p.y, -p.x * sinA + p.z * cosA};
}

Point3D RotatingLine::rotateZ(const Point3D& p, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return {p.x * cosA - p.y * sinA, p.x * sinA + p.y * cosA, p.z};
}

RotatingLine::RotatingLine(Point3D start, Point3D end, int numInternalPoints) {
    generatePoints(start, end, numInternalPoints);
    datafile = "line_data.dat";
}

void RotatingLine::generatePoints(Point3D start, Point3D end, int numPoints) {
    points.clear();
    points.push_back(start);
    for (int i = 1; i <= numPoints; i++) {
        double t = static_cast<double>(i) / (numPoints + 1);
        points.push_back({
            start.x + t * (end.x - start.x),
            start.y + t * (end.y - start.y),
            start.z + t * (end.z - start.z)
        });
    }
    points.push_back(end);
}

void RotatingLine::applyRotation(double x_deg, double y_deg, double z_deg) {
    double x_rad = x_deg * M_PI / 180.0;
    double y_rad = y_deg * M_PI / 180.0;
    double z_rad = z_deg * M_PI / 180.0;
    for (auto& p : points) {
        p = rotateX(p, x_rad);
        p = rotateY(p, y_rad);
        p = rotateZ(p, z_rad);
    }
}

void RotatingLine::saveData() {
    std::ofstream out(datafile);
    for (const auto& p : points) {
        out << p.x << " " << p.y << " " << p.z << std::endl;
    }
    out.close();
}

void RotatingLine::plotInteractive() {
    saveData();
    std::ofstream script("plot.gnu");
    script << "set title '3D Line with Interactive Rotation'\n"
           << "set xlabel 'X'\n"
           << "set ylabel 'Y'\n"
           << "set zlabel 'Z'\n"
           << "set grid\n"
           << "set mouse\n"
           << "set view 60, 30\n"
           << "splot '" << datafile << "' with linespoints lc rgb 'red' pt 7\n"
           << "pause -1 'Press any key to exit'\n";
    script.close();
    system("gnuplot plot.gnu");
}
