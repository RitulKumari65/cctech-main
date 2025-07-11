#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include "geometry.h"

class Cylinder {
private:
    double x, y, z;     // Center of bottom base
    double radius, height;
    int segments;       // Number of points to approximate the circle
    double rotationX, rotationY, rotationZ;

    std::vector<Point3D> getDrawable();

public:
    Cylinder(double x, double y, double z, double radius, double height, int segments = 30);
    void setRotation(double rotX_deg, double rotY_deg, double rotZ_deg);
    void saveData();
    void plotInteractive();
};

#endif
