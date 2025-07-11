#ifndef ROBOT_H
#define ROBOT_H

#include "cuboid.h"
#include "sphere.h"
#include "cylinder.h"

class Robot {
public:
    Robot(double x, double y, double z);

    void setTranslation(double tx, double ty, double tz);
    void setRotation(double rotX, double rotY, double rotZ);
    void setScaling(double sx, double sy, double sz);

    void saveData();
    void plotInteractive();

private:
    Cuboid torso;
    Sphere head;
    Cylinder leftArm, rightArm;
    Cylinder leftLeg, rightLeg;

    double translateX, translateY, translateZ;
    double rotateX, rotateY, rotateZ;
    double scaleX, scaleY, scaleZ;

    Point3D applyTransform(const Point3D& point);
};

#endif // ROBOT_H
