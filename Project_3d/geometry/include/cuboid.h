#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include "geometry.h"

class Cuboid {
private:
    double x, y, z;    // Origin point
    double l, b, h;    // Length, breadth, height
    double rotationX, rotationY, rotationZ;  // Rotation angles

public:
    Cuboid(double x, double y, double z, double l, double b, double h);
    void setRotation(double rotX_deg, double rotY_deg, double rotZ_deg);
    void saveData();
    void plotInteractive();

    std::vector<Point3D> getDrawable();  // **Moved to public section**
};

// Cube subclass (inherits from Cuboid)
class Cube : public Cuboid {
public:
    Cube(double x, double y, double z, double side);
};

#endif // CUBOID_H
