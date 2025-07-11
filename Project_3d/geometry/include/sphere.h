#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "geometry.h"

class Sphere {
private:
    double x, y, z, radius;
    int resolution;  // Number of segments

public:
    Sphere(double x, double y, double z, double radius, int resolution = 20);
    void saveData();
    void plotInteractive();
};

#endif // SPHERE_H
