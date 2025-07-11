#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

class Point3D {
public:
    double x, y, z;
    Point3D(double x = 0, double y = 0, double z = 0);
};

class RotatingLine {
private:
    std::vector<Point3D> points;
    std::string datafile;
    
    Point3D rotateX(const Point3D& p, double angle);
    Point3D rotateY(const Point3D& p, double angle);
    Point3D rotateZ(const Point3D& p, double angle);

public:
    RotatingLine(Point3D start, Point3D end, int numInternalPoints);
    void generatePoints(Point3D start, Point3D end, int numPoints);
    void applyRotation(double x_deg, double y_deg, double z_deg);
    void saveData();
    void plotInteractive();
};

#endif // GEOMETRY_H
