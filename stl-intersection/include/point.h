#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
    float x, y, z;
    Point(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

#endif