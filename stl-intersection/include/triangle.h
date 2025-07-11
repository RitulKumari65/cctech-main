#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "point.h"

class Triangle {
public:
    Point p1, p2, p3;
    Triangle(Point p1 = Point(), Point p2 = Point(), Point p3 = Point()) : p1(p1), p2(p2), p3(p3) {}
};

#endif